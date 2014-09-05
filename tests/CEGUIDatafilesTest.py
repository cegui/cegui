#!/usr/bin/env python

# This script runs with both python2 and python3

# Copyright (C) 2014 Paul D Turner & The CEGUI Development Team
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

import glob
import sys
import os
import os.path

try:
    from lxml import etree

except ImportError:
    print("WARNING: Failed to import lxml. This most probably means you "
          "haven't got lxml installed. Run `pip install lxml` or "
          "`yum install python-lxml` and then retry.")

    # We intentionally exit with 0, failure to find lxml doesn't mean datafiles
    # are sure to be invalid.
    sys.exit(0)


def validate_file(schema_path, file_path):
    # TODO: Yeah yeah, we load the XMLSchema over and over, who cares...
    schema = etree.XMLSchema(file = schema_path)
    if schema.validate(etree.parse(file_path)):
        return True, []

    else:
        return False, schema.error_log


def gather_files(xml_files, datafiles_path, schema_path, globs):
    for glob_ in globs:
        for file_ in glob.glob(os.path.join(datafiles_path, glob_)):
            xml_files[file_] = os.path.join(datafiles_path, schema_path)


def main():
    xml_files = {}

    datafiles_path = os.path.join(os.path.dirname(__file__), "..", "datafiles")

    gather_files(xml_files, datafiles_path,
                 "xml_schemas/Animation.xsd", ["animations/*.anims"])
    gather_files(xml_files, datafiles_path,
                 "xml_schemas/Font.xsd", ["fonts/*.font"])
    gather_files(xml_files, datafiles_path,
                 "xml_schemas/Imageset.xsd", ["imagesets/*.imageset"])
    gather_files(xml_files, datafiles_path,
                 "xml_schemas/GUILayout.xsd", ["layouts/*.layout", "layouts/*.wnd"])
    gather_files(xml_files, datafiles_path,
                 "xml_schemas/Falagard.xsd", ["looknfeel/*.looknfeel"])
    gather_files(xml_files, datafiles_path,
                 "xml_schemas/GUIScheme.xsd", ["schemes/*.scheme"])

    failed_xml_files = 0

    for file_path in sorted(xml_files):
        schema_path = xml_files[file_path]

        result, error_log = validate_file(schema_path, file_path)

        file_path = os.path.relpath(file_path, datafiles_path)
        schema_path = os.path.relpath(schema_path, datafiles_path)

        if result:
            print("%s is valid against %s" % (file_path, schema_path))

        else:
            print("")
            print("ERROR: %s failed validation against %s" % (file_path, schema_path))
            for error in iter(error_log):
                print(error)

            print("")

            failed_xml_files += 1

    print("")

    if failed_xml_files == 0:
        print("All %i files are valid!" % (len(xml_files)))
        sys.exit(0)

    else:
        print("%i files out of %i (%f %%) failed to validate!" % (failed_xml_files, len(xml_files), 100.0 * float(failed_xml_files) / float(len(xml_files))))
        sys.exit(1)


if __name__ == "__main__":
    main()
