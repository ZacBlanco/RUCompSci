import game
from game import APP as g

game.APP_TITLE = "Intro to artificial intelligence"

TILE1 = None
G = None
def setup(screen, args):
    global TILE1, G
    TILE1 = Tile('blocked', 10, 20, [50, 50])
    G = Grid.gen_grid(0, 0, args['w'], args['h'], args['tw'], args['th'])
def LOOP(d, s):
    global TILE1, G
    # s.fill([10, 10, 10])
    G.draw(s)


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
        g.matrix = [[ Tile('unblocked', x + (i*width), y + (j*height), [width, height]) for j in range(ty)] for i in range(tx)]
        for row in g.matrix:
            for t in range(len(row)):
                if (t % 2 == 0):
                    row[t].update_tile(tt='blocked')
        return g


    @staticmethod
    def from_file(filename):
        pass

    def draw(self, screen):
        for row in self.matrix:
            for t in row:
                # print("x: {} y: {} col: {}".format(t.x, t.y, t.color))
                t.draw(screen)

class Tile(object):
    TYPE_COLORS = {
        'unblocked': [37, 186, 86],
        'blocked': [226, 56, 22],
        'agent': [0, 0, 0],
        'target': [124, 22, 226]
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
    game.add_option('tw', False, 50, 'The number of tiles in each row of the grid')
    game.add_option('th', False, 50, 'The number of tiles in each column of the grid')
    game.run(setup, LOOP)
