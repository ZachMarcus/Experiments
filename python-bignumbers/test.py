from decimal import *

def calcF(n):
	return Decimal(1000) * (Decimal(n))

def calcH(n):
	if n%2 == 0:
		return Decimal(1)
	else:
		return Decimal(n) * Decimal(n).log10()

class BigNumComputeComparison(object):
	def __init__(self, f, h, precision, num_range=None):
		self.f = f
		self.h = h
		self.precision = precision
		if num_range:
			self.num_range = num_range
		else:
			with localcontext() as ctx:
				ctx.prec = self.precision
				self.num_range = []
				for x in range(20000, 20050):
					self.num_range.append(2**x + x)

	def run_comparison(self, numRangeList=None):
		if numRangeList:
			self.num_range = numRangeList
		with localcontext() as ctx:
			ctx.prec = self.precision
			for num in self.num_range:
				if self.f(num) > self.h(num):
					print("F({})\n>\nH({})".format(num, num))
				else:
					print("F({})\n<=\nH({})".format(num, num))



if __name__ == "__main__":
	comparator = BigNumComputeComparison(calcF, calcH, 4096)
	comparator.run_comparison()

