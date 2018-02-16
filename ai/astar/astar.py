import sys
import random
import time
import itertools
import heapq
import math
import game
from game import APP as g
from game_objects import Tile, Grid

game.APP_TITLE = "Intro to artificial intelligence"
DEFAULT_WIDTH = 71
DEFAULT_HEIGHT = 71

def heuristic(source_tile, target_tile):
    '''A set of 2-tuple of the (x, y) location of our targets
    
    Returns:
        int: The heuristic distance between the two tiles (minmium number of x+y movements)
    '''
    return target_tile[1] - source_tile[1] + target_tile[0] - source_tile[0]
    


def setup(screen, args):

    if args['file'] is not '':
        game.g = Grid.from_file(args['file'])
    elif args['generate'] is not '':
        game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
        for i in range(50):
            game.g.dfs_maze()
            game.g.draw(screen)
            g.display.flip()
            time.sleep(0.2)
            game.g.to_file('mazes/maze{}.grid'.format(i))
        sys.exit(0)
    else:
        game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
        game.g.dfs_maze()
        game.g.draw(screen)
        g.display.flip()

    game.open = []
    game.os = {}
    game.closed = {}
    game.source = (1, 1)
    game.target = (game.g.tx - 2, game.g.ty - 2)
    game.g.get(game.source).update(tt="agent")
    game.g.get(game.target).update(tt="target")
    h_add_tile(game.source, heuristic(game.source, game.target))
    game.came_from = {}
    game.g_score = {}
    game.g_score[game.source] = 0
    game.reconstruct = False
    game.last = None
def loop(d, s):

    if len(game.open) > 0:
        score, current = h_pop_tile()
        if current == game.target:
            # done
            game.open = []
            game.reconstruct = True
            return
        game.closed[current] = True
        if game.g.get(current).type == 'path':
            game.g.get(current).color = Tile.TYPE_COLORS['explored']
        
        for neighbor in game.g.get(current).get_walls(game.g.tx, game.g.ty):

            if game.g.get(neighbor).type != 'wall':
                game.g.get(neighbor).color = Tile.TYPE_COLORS['reached']
            game.g_score[neighbor] = math.inf
            if neighbor in game.closed or game.g.get(neighbor).type == 'wall':
                continue
            
            if neighbor not in game.os:
                h_add_tile(neighbor, math.inf)
            
            tent_g = game.g_score[current] + 1 # distance between current and neighbor will always be 1
            if neighbor in game.g_score and tent_g < game.g_score[neighbor]:
                game.came_from[neighbor] = current
                game.g_score[neighbor] = tent_g
                h_add_tile(neighbor, tent_g + heuristic(neighbor, game.target))
            else:
                continue

    elif game.reconstruct:

        if game.last == None:
            game.last = game.target
        # print(game.last)
        # print(game.target)
        game.g.get(game.last).color = Tile.TYPE_COLORS['shortest']
        if game.last in game.came_from:
            game.last = game.came_from[game.last]
        else:
            game.reconstruct = False


    game.g.draw(s)

REMOVED = "NULL_TASK"
ctr = itertools.count()

def h_add_tile(tile, fscore):
    if tile in game.os:
        h_remove_tile(tile)
    tid = next(ctr)
    ent = [fscore, tid, tile]
    game.os[tile] = ent
    heapq.heappush(game.open, ent)

def h_remove_tile(tile):
    ent = game.os.pop(tile)
    ent[2] = REMOVED

def h_pop_tile():
    while game.open:
        fscore, tid, tile = heapq.heappop(game.open)
        if tile is not REMOVED:
            game.os.pop(tile)
            return fscore, tile
    raise KeyError("Attempted to pop empty priority queue")
        

if __name__ == "__main__":
    # add options here
    game.add_option('tw', False, DEFAULT_WIDTH, 'The number of tiles in each row of the grid')
    game.add_option('th', False, DEFAULT_HEIGHT, 'The number of tiles in each column of the grid')
    game.add_option('file', False, "", 'If specified, will read from the file instead of generating a new maze')
    game.add_option('generate', False, "", 'This will generate 50 different mazes and save them under a folder names "mazes"')
    game.run(setup, loop)
