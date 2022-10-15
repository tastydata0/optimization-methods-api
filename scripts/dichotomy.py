import json
from math import *
from sys import argv

L0_lower = float(argv[1])
L0_upper = float(argv[2])
function = argv[3]
E = float(argv[4])
l0 = float(argv[5])
target = argv[6].lower()

assert target in ('min', 'max')

FIND_MAX = target == 'max'


output_json = {}

def f(x):
    return eval(function)


ROUND_PRECISION = 12


L = [[L0_lower, L0_upper]]

n = (2/log(2)) * log((L0_upper-L0_lower - E)/(l0-E))

n = ceil(n/2)*2

output_json['n'] = n

x_best = 0
f_best = float('-inf') if FIND_MAX else float('inf')

for i in range(n//2):
    mid = round((L[-1][1] + L[-1][0])/2, ROUND_PRECISION)

    x_left = round(mid - E/2, ROUND_PRECISION)
    x_right = round(mid + E/2, ROUND_PRECISION)


    f_left = round(f(x_left), ROUND_PRECISION)
    f_right = round(f(x_right), ROUND_PRECISION)

    if FIND_MAX:
        if f_left > f_best:
            f_best = f_left
            x_best = x_left
        if f_right > f_best:
            f_best = f_right
            x_best = x_right

        if f_left > f_right:
            L.append([L[-1][0], x_right])
        elif f_left < f_right:
            L.append([x_left, L[-1][1]])
        else:
            L.append([x_left, x_right])
    else:
        if f_left < f_best:
            f_best = f_left
            x_best = x_left
        if f_right < f_best:
            f_best = f_right
            x_best = x_right

        if f_left < f_right:
            L.append([L[-1][0], x_right])
        elif f_left > f_right:
            L.append([x_left, L[-1][1]])
        else:
            L.append([x_left, x_right])

    output_json[f'L{len(L)*2 - 2}'] = L[-1]

output_json['x*'] = x_best
output_json['f*'] = f_best
output_json['status'] = 'success'

print(json.dumps(output_json))
