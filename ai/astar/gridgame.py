import random
import game
import time
from game import APP as g

game.APP_TITLE = "Intro to artificial intelligence"
DEFAULT_WIDTH = 70
DEFAULT_HEIGHT = 49


def setup(screen, args):
    game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
    for row in game.g.matrix:
            for col in row:
                col.update(tt='wall')
    game.start = (1, 1)
    game.dir_stack = []
    game.curr_tile = game.start
    game.dirs = game.g.gen_rand_dirs()
    for d in game.dirs:
        game.dir_stack.append((game.curr_tile, d))
    # game.g.dfs_maze()

def LOOP(d, s):

    


    (x, y), direction = game.dir_stack.pop()
    if direction == 0:
        if (y - 2 <= 0):
            return
        if game.g.matrix[y-2][x].type != 'path':
            game.g.matrix[y-2][x].update(tt='path')
            game.g.matrix[y-1][x].update(tt='path')
            [game.dir_stack.append(l) for l in game.g.gen_dt_list(x, y-2)]
    elif direction == 1:
        if (x + 2 >= game.g.tx -1):
            return
        if game.g.matrix[y][x+2].type != 'path':
            game.g.matrix[y][x+2].update(tt='path')
            game.g.matrix[y][x+1].update(tt='path')
            [game.dir_stack.append(l) for l in game.g.gen_dt_list(x+2, y)]
    elif direction == 2:
        if (y + 2 >= game.g.ty -1):
            return
        if game.g.matrix[y+2][x].type != 'path':
            game.g.matrix[y+2][x].update(tt='path')
            game.g.matrix[y+1][x].update(tt='path')
            [game.dir_stack.append(l) for l in game.g.gen_dt_list(x, y+2)]
    else:
        if (x - 2 <= 0):
            return
        if game.g.matrix[y][x-2].type != 'path':
            game.g.matrix[y][x-2].update(tt='path')
            game.g.matrix[y][x-1].update(tt='path')
            [game.dir_stack.append(l) for l in game.g.gen_dt_list(x-2, y)]
    game.g.draw(s)


class Grid(object):
    def __init__(self, x, y, g_width, g_height, tx, ty):
        '''Create a grid of tiles

        Arguments:
            x: X location of grid (top left)
            y: Y location of grid (top left)
            g_width: The grid width (in pixels)
            g_height: The grid height (in pixels)
            tx: Number of tiles across
            ty: Number of tiles down
        '''
        self.x = x
        self.y = y
        self.gw = g_width
        self.gh = g_height
        self.tx = tx
        self.ty = ty
        self.matrix = None
    
    def get(self, location):
        '''Returns the tile at ``location``

        Arguments:
            location (tuple): A 2-tuple of values indicating the (x, y) coordinate of the tile in the grid

        Returns:
            Tile: The tile at the specified location
        '''
        return self.matrix[location[1]][location[0]]

    @staticmethod
    def gen_grid(x, y, gw, gh, tx, ty):
        '''Generate a blank grid

        Arguments:
            x: The initial horizontal location in the window for the grid (pixels)
            y: The initial top location of the grid (pixels)
            gw: The total width of the grid in pixels
            gh: The total height of the grid in pixels
            tx: The number of tiles in each row
            ty: The number of tiles in each column
        '''
        g = Grid(x, y, gw, gh, tx, ty)
        # Tile size:
        width = float(gw) / tx
        height = float(gh) / ty
        g.tw = width # tile width (pixels)
        g.th = height # tile height (pixels)
        g.matrix = [[ Tile('wall', i, j, x + (i*width), y + (j*height), [width, height]) for i in range(tx)] for j in range(ty)]

        start = (0,0)
        g.matrix[start[0]][start[1]].update(tt='path')
        
        return g
    
    def random_maze(self, prob):
        for row in self.matrix:
            for col in row:
                col.update(tt='wall')
        
        for row in self.matrix:
            for col in row:
                if random.random() < prob:
                    col.update(tt='path')
    
    def __gen_dt_list(self, tx, ty):
        l = []
        dirs = self.gen_rand_dirs()
        for d in dirs:
            l.append(((tx, ty), d))
        return l
    def gen_dt_list(self, tx, ty):
        l = []
        dirs = self.gen_rand_dirs()
        for d in dirs:
            l.append(((tx, ty), d))
        return l
    
    def __dfs_maze_norecurse(self, sx, sy):
        start = (sx, sy)
        dir_stack = []
        curr_tile = start
        dirs = self.gen_rand_dirs()
        for d in dirs:
            dir_stack.append((curr_tile, d))


        while len(dir_stack) > 0:
            (x, y), direction = dir_stack.pop()
            if direction == 0:
                if (y - 2 <= 0):
                    continue
                if self.matrix[y-2][x].type != 'path':
                    self.matrix[y-2][x].update(tt='path')
                    self.matrix[y-1][x].update(tt='path')
                    [dir_stack.append(l) for l in self.__gen_dt_list(x, y-2)]
            elif direction == 1:
                if (x + 2 >= self.tx -1):
                    continue
                if self.matrix[y][x+2].type != 'path':
                    self.matrix[y][x+2].update(tt='path')
                    self.matrix[y][x+1].update(tt='path')
                    [dir_stack.append(l) for l in self.__gen_dt_list(x+2, y)]
            elif direction == 2:
                if (y + 2 >= self.ty -1):
                    continue
                if self.matrix[y+2][x].type != 'path':
                    self.matrix[y+2][x].update(tt='path')
                    self.matrix[y+1][x].update(tt='path')
                    [dir_stack.append(l) for l in self.__gen_dt_list(x, y+2)]
            else:
                if (x - 2 <= 0):
                    continue
                if self.matrix[y][x-2].type != 'path':
                    self.matrix[y][x-2].update(tt='path')
                    self.matrix[y][x-1].update(tt='path')
                    [dir_stack.append(l) for l in self.__gen_dt_list(x-2, y)]



    def __dfs_maze_recurse(self, x, y):

        for direction in self.gen_rand_dirs():
            if direction == 0:
                if (y - 2 <= 0):
                    continue
                if self.matrix[y-2][x].type != 'path':
                    self.matrix[y-2][x].update(tt='path')
                    self.matrix[y-1][x].update(tt='path')
                    self.__dfs_maze_recurse(x, y-2)
            elif direction == 1:
                if (x + 2 >= self.tx -1):
                    continue
                if self.matrix[y][x+2].type != 'path':
                    self.matrix[y][x+2].update(tt='path')
                    self.matrix[y][x+1].update(tt='path')
                    self.__dfs_maze_recurse(x+2, y)
            elif direction == 2:
                if (y + 2 >= self.ty -1):
                    continue
                if self.matrix[y+2][x].type != 'path':
                    self.matrix[y+2][x].update(tt='path')
                    self.matrix[y+1][x].update(tt='path')
                    self.__dfs_maze_recurse(x, y+2)
            else:
                if (x - 2 <= 0):
                    continue
                if self.matrix[y][x-2].type != 'path':
                    self.matrix[y][x-2].update(tt='path')
                    self.matrix[y][x-1].update(tt='path')
                    self.__dfs_maze_recurse(x-2, y)

    def dfs_maze(self):
        # self.__dfs_maze_recurse(0, 0)
        self.__dfs_maze_norecurse(1, 1)
        self.matrix[1][1].update(tt="path")
        



    def gen_rand_dirs(self):
        nums = list(range(4))
        random.shuffle(nums)
        return nums


    def primms_maze(self):

        for row in self.matrix:
            for col in row:
                col.update(tt='wall')

        visited = {}
        frontier = {}
        root_tile = self.matrix[0][0]
        root_tile.update(tt='path')
        visited[root_tile.get_tile_loc()] = True
        wall_list = root_tile.get_walls(self.gw, self.gh)
        for wall in wall_list:
            frontier[wall] = True

        while len(wall_list) > 0:
            i = random.randint(0, len(wall_list) - 1)
            win = self.get(wall_list[i])
            t = win.get_walls(self.tx,self.ty)
            for x, y, in t:
                if (x, y) not in visited and (x, y) not in frontier :
                    wall_list.append((x, y))
                    frontier[(x, y)] = win.get_tile_loc()


            types = {}
            for tile in t:
                tp = self.get(tile).type
                if tp not in types:
                    types[tp] = 1
                else:
                    types[tp] += 1

            if 'wall' not in types or types['wall'] / len(t) >= 1.5 / len(t):
                win.update(tt='path')
        
            visited[win.get_tile_loc()] = True
            wall_list.pop(i)
            frontier.pop(win.get_tile_loc())



        self.matrix[0][0].update(tt='agent')        
        self.matrix[self.ty-1][self.tx-1].update(tt='target')



        return None

    @staticmethod
    def from_file(filename):
        pass

    def draw(self, screen):
        for row in self.matrix:
            for t in row:
                t.draw(screen)

        for col in range(self.tx):
            g.draw.line(screen, [0, 0, 0], [self.x + self.tw*col, self.y], [self.x + self.tw*col, self.y+self.gh], 3)
        
        for row in range(self.ty):
            g.draw.line(screen, [0, 0, 0], [self.x, self.y + self.th*row], [self.x + self.gw, self.y + self.th*row], 3)


class Tile(object):
    TYPE_COLORS = {
        'path': [37, 186, 86],
        'wall': [226, 56, 22],
        'agent': [255, 255, 255],
        'target': [124, 22, 226]
    }

    def __init__(self, tt, gx, gy, xloc, yloc, size):
        self.gx = gx
        self.gy = gy
        self.type = tt
        self.x = xloc
        self.y = yloc
        self.size = size
        self.rect = g.Rect(xloc, yloc, size[0], size[1])
        if tt not in Tile.TYPE_COLORS: raise ValueError("Bad tile type: {}".format(tt))
        self.color = Tile.TYPE_COLORS[tt]
    
    def draw(self, screen):
        screen.fill(self.color, self.rect)
    
    def update(self, tt=None, xloc=None, yloc=None, size=None):
        if tt is not None:
            if tt not in Tile.TYPE_COLORS: raise ValueError("Bad tile type: {}".format(tt))
            self.color = Tile.TYPE_COLORS[tt]
            self.type = tt

    def get_tile_loc(self):
        '''Returns the locations of the tile in x/y grid coordinates'''
        return (self.gx, self.gy)

    def get_walls(self, gw, gh):
        '''Returns the tiles which surround the current tile'''
        neighbors = []

        if self.gx > 0:
            neighbors.append((self.gx - 1, self.gy))
        if self.gx < gw - 1:
            neighbors.append((self.gx + 1, self.gy))
        if self.gy > 0:
            neighbors.append((self.gx, self.gy - 1))
        if self.gy < gh - 1:
            neighbors.append((self.gx, self.gy + 1))

        return neighbors
    
    def __str__(self):
        s = '''X: {}
               Y: {}
               W: {}
               H: {}
               COLOR: {}
        '''.format(self.x, self.y, self.size[0], self.size[1], self.color)
        return s

if __name__ == "__main__":
    # add options here
    game.add_option('tw', False, DEFAULT_WIDTH, 'The number of tiles in each row of the grid')
    game.add_option('th', False, DEFAULT_HEIGHT, 'The number of tiles in each column of the grid')
    game.run(setup, LOOP)
