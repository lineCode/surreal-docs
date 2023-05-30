#
# Copyright © 2022, Medelfor, Limited. All rights reserved.
# For any additional information refer to https://medelfor.com
#

#1 - mbs root
#2 - json path

import json
import sys

mbs_root = sys.argv[1]
json_path = sys.argv[2]

f = open(mbs_root + "/request.json", "r")

value = json.load(f)

for k in json_path.split('.'):
    value = value[k]

print(value)
