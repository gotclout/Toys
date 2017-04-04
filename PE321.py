#!/usr/bin/env python

from math import *

MAX_TRI = 9999999L

triangles = []

def next_pos(mn, pos):
  if mn > triangles[MAX_TRI - 1]:
    return -1
  else:
    maxv = MAX_TRI - 1
    minv = pos
    mid = minv + (maxv - minv) / 2
    while triangles[mid] != mn and minv < maxv:
      if triangles[mid] < mn :
        minv = mid + 1
      else :
        maxv = mid - 1
      mid = minv + (maxv - minv) / 2
  return mid

def gen_triangles(offset):
  triangles[:] = []
  i = 1L + offset * MAX_TRI
  bound = i + MAX_TRI
  print "Generating %i through %i " % (i, bound)
  while i <= bound:
    triangles.append((i * (i + 1L)) / 2L)
    i += 1L
  print "Max value = %i " % (triangles[MAX_TRI - 1])

def pe321():
  offset = pos = tn = total = count = mn = 0L
  n = 1L
  while count < 40:
    mn = 2L * n + n * n
    while mn % 3 != 0 and mn % 9 != 1:
      n += 1L
      mn = 2L * n + n * n
    tn = 1 + 8 * mn··
    #pos = next_pos(mn, pos)
    if sqrt(tn) % 1 == 0:
      print "%i.\tM(%i) = %i is a triangule number" % (count + 1, n, mn)
      count += 1L
      total += n
    #  if n == 1L:
    n += 1L
    #  else:
    #    n += tn / (2 * (n * n))
    #else:
    #  n += 1L·
  print "The sum of the first %i terms = %i" % (count, total)

pe321()
