#!/usr/bin/env python3

import argparse

from collections import deque
import logging
import os
import os.path
import shutil

from pprint import pprint

import pefile

class Library:
    INPUT = 1
    SYSTEM = 2
    THIRDPARTY = 3

    def __init__(self, name, path, system, deps):
        self.name = name
        self.path = path
        self.kind = system
        self.deps = deps


        

class LibraryResolver:
    def __init__(self, libdirs):
        self.libdirs = libdirs
        self.kind_paths = self._make_system_paths()
        self.queue = deque()
        self.found = dict()

    @staticmethod
    def _make_system_paths():
        system_root = os.getenv("SystemRoot")
        if system_root:
            system_paths = [
                    os.path.join(system_root, "system32"),
                    system_root
            ]
            logging.info("Using system path: %s", ", ".join(system_paths))
            return system_paths
        else:
            logging.warning("%%SystemRoot%% not set")
            return []

    def process(self, files):
        for f in files:
            self.process_file(f)

        while len(self.queue) > 0:
            file_name = self.queue.popleft()

            if file_name in self.found:
                continue

            lib_info = self.get_info(file_name)
            self.record(lib_info)
            # for dep in lib_info.deps:
            #     self.queue.append(dep)
            #
            # self.found[file_name] = lib_info

    def process_file(self, path):
        base_name = os.path.basename(path)
        deps = self.scan_dependencies(path)
        info = Library(base_name, path, Library.INPUT, deps)
        self.record(info)

    def record(self, info):
        for dep in info.deps:
            self.queue.append(dep)
        self.found[info.name] = info

    def print_files(self):
        for fi in self.found.values():
            if fi.kind == Library.THIRDPARTY:
                print(fi.path)

    def copy(self, dest):
        for fi in self.found.values():
            if fi.kind == Library.THIRDPARTY:
                logging.info("Copying %s", fi.path)
                shutil.copy(fi.path, dest)

    def get_info(self, file_name, kind=Library.THIRDPARTY):
        logging.debug("Processing %s", file_name)

        system_lib = self.find_lib(file_name, self.kind_paths)
        if system_lib:
            logging.debug("  Found system library: %s", system_lib)
            return Library(file_name, system_lib, Library.SYSTEM, [])

        path = self.find_lib(file_name, self.libdirs)
        if path:
            logging.debug("  Found: %s", path)
            deps = self.scan_dependencies(path)
            logging.debug("  Dependencies: %s", " ".join(deps))
            return Library(file_name, path, kind, deps)

        raise RuntimeError("Cannot find DLL {0}".format(file_name))

    def scan_dependencies(self, file_name):
        pe = pefile.PE(file_name)
        return [entry.dll.decode("utf-8") for entry in pe.DIRECTORY_ENTRY_IMPORT]

    def find_lib(self, name, dirs):
        for d in dirs:
            path = os.path.join(d, name)
            if os.path.exists(path):
                return path
        return None








def main():
    parser = argparse.ArgumentParser(description="Copy all DLLs")
    parser.add_argument("files", type=str, nargs="+",
                        help="Files to scan")
    parser.add_argument("-L", "--libdir", type=str, action="append",
                        help="Source directory containing library files")
    parser.add_argument("-d", "--dest", type=str,
                        help="Destination directory")
    parser.add_argument("-V", "--verbose", type=str, action="store", nargs="?",
                        default="error", const="warning",
                        help="Verbosity level")
    parser.add_argument("--dry-run", action='store_true',
                        help="Do not copy anything; print files that would be copied")
    args = parser.parse_args()
    logging.basicConfig(level=getattr(logging, args.verbose.upper(), None), format="%(levelname)s %(message)s")

    d = LibraryResolver(args.libdir or [])
    d.process(args.files)
    if args.dry_run:
        d.print_files()
    else:
        d.copy(args.dest)



if __name__ == "__main__":
    main()
