def gcd(a,b):
    """ Eucild's algorithm, finds greatest common divisor of a and b"""
    while b:
        a, b = b, a % b
    return a

def to_long(x):
    """ works like long(), but returns 0 if x is empty"""
    if x:
        return long(x)
    else:
        return 0l


class rational:
	def __init__(self, n, d):
		self.n = n
		self.d = d
		self.normalize()

	def from_string(num_str, plen):
		# cut off "0."
		assert(num_str[:2] == '0.')
		num_str = num_str[2:]

		n = to_long(num_str) - to_long(num_str[:-plen])
		d = 10l**len(num_str)-10l**(len(num_str)-plen)
		return rational(n, d)

	from_string = staticmethod(from_string)

	def normalize(self):
		d = gcd(self.n, self.d)
		self.n = self.n / d
		self.d = self.d / d
		return self

	def __iadd__(self, r):
		self.n = self.n * r.d + self.d * r.n
		self.d = self.d * r.d
		return self

	def __isub__(self, r):
		self.n = self.n * r.d - self.d * r.n
		self.d = self.d * r.d
		return self

	def __imul__(self, r):
		self.n *= r.n
		self.d = self.d * r.d
		return self

	def __idiv__(self, r):
		self.n *= r.d
		self.d *= r.n
		return self

	def __cmp__(self, r):
		if (self.n == r.n) and (self.d == r.d): return 0
		if (self.n * r.d) < (self.d * r.n): return 1
		return -1

	def eval(self):
		return float(self.n) / float(self.d)

	def __str__(self):
		return str(self.n) + '/' + str(self.d)


input = """2 0.318
1 0.3
2 0.09
6 0.714285
3 0.4459
3 0.44594
21 0.23837209302325581395348"""

def cleanup(l):
	return l[1], int(l[0])

for line in input.split("\n"):
	r = apply(rational.from_string, cleanup(line.split()))
	print line, ' ==>', r, ',', r.eval()

phi = rational(1,1)
print phi, 1
for i in range(80):
	r = rational(1,1)
	r /= phi
	phi = r
	phi += rational(1,1)
	print phi, phi.eval()
