#!/usr/bin/env python

import argparse, shutil, sys
from pathlib import Path


MAIN_CPP_TEMPLATE = """#include "{proj_name}.hpp"

#include <iostream>

void part1() {{}}

void part2() {{}}

int main() {{
    std::cout << "Day {day}: TODO";
    part1();
    part2();
}}
"""

HEADER_TEMPLATE = """#pragma once

#include "aoc/aoc.hpp"

namespace {proj_name} {{}}  // namespace {proj_name}
"""

TEST_CPP_TEMPLATE = """#include "{proj_name}"

#include <gtest/gtest.h>

TEST(Day{day}, HelloWorld) {{
    ASSERT_TRUE(true);
}}
"""

README_TEMPLATE = """# Day {day}{title}

## Part One

## Part Two
"""


def bail(msg: str, code=1):
    print(f"error: {msg}")
    sys.exit(code)


def confirm(prompt: str | None = None):
    if prompt is None:
        prompt = "Are you sure you want to continue?"
    choice = input(f"{prompt} [y/N] ").lower()
    return choice == "y" or choice == "yes"


def status(msg: str, level: int = 1):
    indent = "  " * level
    print(f"{indent}- {msg}")


parser = argparse.ArgumentParser(
    description="Generate an Advent of Code project directory for a given day",
)
parser.add_argument(
    "-n",
    "--num",
    metavar="DAY",
    dest="day",
    type=int,
    required=True,
    help="the day number for this project",
)
parser.add_argument(
    "-d",
    "--parent-dir",
    metavar="DIR",
    type=str,
    help="optionally specify a different parent directory for the project",
)
parser.add_argument(
    "-t",
    "--title",
    type=str,
    help="optional title for the day's problem (used in README template)",
)


def validate_parent(path: Path):
    if not path.exists() or not path.is_dir():
        bail(f"parent directory `{path.resolve()}` does not exist")

    expected_files = [path.joinpath("CMakeLists.txt"), path.joinpath(".gitignore")]
    for f in expected_files:
        if not f.exists():
            bail(
                "parent directory is not suitable\n"
                f"  - expected to find `{f.name}` in `{f.parent.resolve()}`"
            )


if __name__ == "__main__":
    args = parser.parse_args()

    parent_dir = Path(args.parent_dir or "..")
    validate_parent(parent_dir)

    day = args.day
    proj_name = f"day{day}"
    proj_title = args.title or ""
    proj_dir = parent_dir.joinpath(proj_name)
    proj_include = proj_dir.joinpath("include")

    if proj_dir.exists():
        if proj_dir.is_file():
            bail("destination path exists and is not a directory")
        print(
            f"WARNING: the destination directory `{proj_dir.resolve()}` already exists"
        )
        print("The ENTIRE DIRECTORY will be deleted and cannot be recovered!\n")
        if not confirm():
            sys.exit(0)

    file_contents = {
        proj_dir.joinpath("main.cpp"): MAIN_CPP_TEMPLATE.format(
            proj_name=proj_name, day=day
        ),
        proj_include.joinpath(f"{proj_name}.hpp"): HEADER_TEMPLATE.format(
            proj_name=proj_name
        ),
        proj_dir.joinpath(f"{proj_name}_test.cpp"): TEST_CPP_TEMPLATE.format(
            proj_name=proj_name, day=day
        ),
        proj_dir.joinpath("README.md"): README_TEMPLATE.format(
            day=day, title=f": {proj_title}" if proj_title else ""
        ),
    }

    directories = [
        proj_dir,
        proj_include,
    ]

    print(f"\nCreating solution at: {proj_dir.resolve()}\n")

    if proj_dir.exists():
        status("Deleting existing directory")
        shutil.rmtree(proj_dir)

    status("Creating directory structure")
    for dir in directories:
        status(str(dir.resolve()), 2)
        dir.mkdir()

    status("Writing files")
    for path, content in file_contents.items():
        status(str(path.resolve()), 2)
        with open(path, "w") as f:
            f.write(content)

    print("\nProject successfully created!")
    print(
        f"\nBe sure to add `{proj_name}` to `solutions` "
        "in CMakeLists.txt and add a link in the README.\n"
    )
