import game
from game import APP as g

game.APP_TITLE = "Intro to artificial intelligence"

TILE1 = None
G = None
def setup(screen):
    global TILE1, G
    TILE1 = Tile('blocked', 10, 20, [50, 50])
    G = Grid.gen_grid(0, 0, 300, 300, 20, 20)
def LOOP(d, s):
    global TILE1, G
    # s.fill([10, 10, 10])
    # TILE1.draw(s)
    #G.draw(s)
    t = Tile('blocked', 50, 50, (25, 25))
    t.draw(s)


class Grid(object):
    def __init__(self, x, y, g_width, g_height, nx, ny):
        '''Create a grid of tiles

        Arguments:
            x: X location of grid (top left)
            y: Y location of grid (top left)
            g_width: The grid width (in pixels)
            g_height: The grid height (in pixels)
            nx: Number of tiles across
            ny: Number of tiles down
        '''
        self.width = nx
        self.height = ny
        self.matrix = None
    
    @staticmethod
    def gen_grid(x, y, gw, gh, tx, ty):
        g = Grid(x, y, gw, gh, tx, ty)
        # Tile size:
        width = float(tx) / gw
        height = float(ty) / gh
        g.matrix = [[ Tile('unblocked', x + (i*width), y + (j*height), [width, height]) for j in range(ty)] for i in range(tx)]
        for row in g.matrix:
            for t in row:
                if (t.x % 2 == 0):
                    t.update_tile(tt='blocked')
        return g


    @staticmethod
    def from_file(filename):
        pass

    def draw(self, screen):
        for row in self.matrix:
            for t in row:
                print("x: {} y: {} col: {}".format(t.x, t.y, t.color))
                t.draw(screen)

class Tile(object):
    TYPE_COLORS = {
        'unblocked': [107, 163, 255],
        'blocked': [214, 49, 76]
    }

    def __init__(self, tt, xloc, yloc, size):
        self.type = tt
        self.x = xloc
        self.y = yloc
        self.size = size
        self.rect = g.Rect(xloc, yloc, size[0], size[1])
        if tt not in Tile.TYPE_COLORS: raise ValueError("Bad tile type: {}".format(tt))
        self.color = Tile.TYPE_COLORS[tt]
    
    def draw(self, screen):
        screen.fill(self.color, self.rect)
    
    def update_tile(self, tt=None, xloc=None, yloc=None, size=None):
        if tt is not None:
            if tt not in Tile.TYPE_COLORS: raise ValueError("Bad tile type: {}".format(tt))
            self.color = Tile.TYPE_COLORS[tt]

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

    game.run(setup, LOOP)
