class Grid:
	"""
		Args:
			width (int): Grid's width.
			height (int): Grid's height.

		Attributes:
			grid (list): The list will hold the position empty or not information.
			width (int): Grid's width
			height (int): Grid's height
	"""

	def __init__(self, width, height):
		self.grid = [False] * (width * height)
		self.width = width
		self.height = height

	def __str__(self):
		output = "Grid {}x{} Information\n".format(self.width, self.height)
		for row in range(self.height):
			for col in range(self.width):
				output += "Status of %d: " % (row * self.width + col)
				output += "NotEmpty, " if self.grid[row *
												   self.width + col] else "Empty, "
			output += "\n"

		return output

	def find_blank(self, width, height):
		"""
			Args:
				width (int): The item's width you can call width item.GetItemSize()[0]
				height (int): The item's height you can call width item.GetItemSize()[1]

			Returns:
				int: The return value would be an int if successful. Otherwise -1.
		"""
		if width > self.width or height > self.height:
			return -1

		for row in range(self.height):
			for col in range(self.width):
				index = row * self.width + col
				if self.is_empty(index, width, height):
					return index

		return -1

	def put(self, pos, width, height):
		"""
			Args:
				pos (int): Position of the item to put.
				width (int): The item's width you can call width item.GetItemSize()[0]
				height (int): The item's height you can call width item.GetItemSize()[1]

			Returns:
				bool: The return value. True for success, False otherwise.
		"""
		if not self.is_empty(pos, width, height):
			return False

		for row in range(height):
			start = pos + (row * self.width)
			self.grid[start] = True
			col = 1
			while col < width:
				self.grid[start + col] = True
				col += 1

		return True

	def clear(self, pos, width, height):
		"""
			Args:
				pos (int): Position of the item to put.
				width (int): The item's width you can call width item.GetItemSize()[0]
				height (int): The item's height you can call width item.GetItemSize()[1]

			Returns:
				There is nothing to return
		"""
		if pos < 0 or pos >= (self.width * self.height):
			return

		for row in range(height):
			start = pos + (row * self.width)
			self.grid[start] = True
			col = 1
			while col < width:
				self.grid[start + col] = False
				col += 1

	def is_empty(self, pos, width, height):
		"""
			Args:
				pos (int): Position of the item to put.
				width (int): The item's width you can call width item.GetItemSize()[0]
				height (int): The item's height you can call width item.GetItemSize()[1]

			Returns:
				bool: The return value. True for success, False otherwise.
		"""
		if pos < 0:
			return False

		row = pos // self.width
		if (row + height) > self.height:
			return False

		if (pos + width) > ((row * self.width) + self.width):
			return False

		for row in range(height):
			start = pos + (row * self.width)            
			if self.grid[start]:                
				return False

			col = 1
			while col < width:
				if self.grid[start + col]:                    
					return False
				col += 1

		return True

	def get_size(self):
		"""
			Returns:
				int: The return value will give you maximum capacity of grid. (width * height)
		"""
		return self.width * self.height

	def reset(self):
		"""
			With this function, you can reset instead of deleting it and create again.
		"""
		self.grid = [False] * (self.width * self.height)