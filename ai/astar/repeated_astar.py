import sys
import random
import time
import itertools
import heapq
import math
import game
from game import APP as g
from game_objects import Tile, Grid
from heap import TileHeap

game.APP_TITLE = "Intro to artificial intelligence"
DEFAULT_WIDTH = 101
DEFAULT_HEIGHT = 101

def heuristic(source_tile, target_tile):
    '''A set of 2-tuple of the (x, y) location of our targets
    
    Returns:
        int: The heuristic distance between the two tiles (minmium number of x+y movements)
    '''
    return abs(target_tile[1] - source_tile[1] + target_tile[0] - source_tile[0])
    


def setup(screen, args):
    '''Run the game setup

    '''
    if args['file'] is not '':
        game.g = Grid.from_file(args['file'])
        print("Game grid loaded from {}".format(args['file']))
    elif args['generate'] is not '':
        game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
        for i in range(50):
            # game.g.dfs_maze()
            game.g.random_maze(.7)            
            game.g.draw(screen)
            g.display.flip()
            time.sleep(0.2)
            game.g.to_file('mazes/maze{}.grid'.format(i))
        sys.exit(0)
    else:
        game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
        # game.g.dfs_maze()
        game.g.random_maze(.7)
        game.g.draw(screen)
        g.display.flip()

    game.min_tb = False
    if args['tbr'] != '':
        game.min_tb = True
    game.ctr = 0
    game.open = TileHeap(min_tb=game.min_tb)
    game.closed = {}
    game.source = (1, 1)
    game.target = (game.g.tx - 2, game.g.ty - 2)
    if args['b'] != '':
        tmp = game.source
        game.source = game.target
        game.target = tmp
    game.g.get(game.source).update(tt="agent")
    game.g.get(game.target).update(tt="target")

    game.expanded = 0 # number of expanded tiles
    game.g_score = {}
    game.g_score[game.source] = 0
    game.reconstruct = False
    game.curr_tile = game.source
    game.search = {}
    game.tree = {}
    game.last = None
    game.astar = True
    game.start = time.time()
    game.break_ctr = 0

def loop(d, s):


    if game.astar:
        game.ctr += 1 # Line 19
        game.g_score[game.curr_tile] = 0 #Line 20
        game.search[game.curr_tile] = game.ctr # Line 21
        game.g_score[game.target] = math.inf # Line 22
        game.search[game.target] = game.ctr # Line 23
        topen, tclosed, last = compute_path(game.curr_tile, game.target, s)
        if len(topen) > 0:
            # Didn't fail
            # while tmp in game.tree:
            #     game.g.get(tmp).color = Tile.TYPE_COLORS['shortest']
            #     tmp = game.tree[tmp]
            print("Started with: {}, Ended On {}".format(game.curr_tile, last))
            game.curr_tile = last
            if game.curr_tile == game.target:
                game.reconstruct = True
                game.astar = False
                print("reconstruct is true")
                game.end = time.time()
                print("Finished running in {} seconds".format(game.end - game.start))
                print("Expanded {} cells".format(game.expanded))
                game.g.get(game.source).update(tt='agent')
        else:
            # Failed Search
            print("FAILED SEARCH. NO PATH")
            game.astar = False
            game.EXIT = True
            # time.sleep(2)
    

    if game.reconstruct:
        if game.last == None:
            game.last = game.target
        # print(game.last)
        # print(game.target)
        game.g.get(game.last).color = Tile.TYPE_COLORS['shortest']
        if game.last in game.tree and game.break_ctr < 2500:
            game.last = game.tree[game.last]
        else:
            game.reconstruct = False
            game.EXIT = True
            # time.sleep(2)
        game.break_ctr += 1

    if not game.reconstruct and not game.astar:
        print("Rerunning setup")
        game.g.draw(s)
        setup(s, game.ARGS)

    # game.g.draw(s)

def compute_path(source_tile, target_tile, s):
    topen = TileHeap(min_tb=game.min_tb)
    tclosed = {}
    topen.push(source_tile, 0, heuristic(source_tile, target_tile)) # Line 25
    last = None # used to return last time searched
    while len(topen) >  0: # this prevents us from getting getting an error on failed search
        if not game.g_score[target_tile] > topen.peek()[0]: # break from loop (Line 2)
            break 
        f, gs, tile = topen.pop() # Remove the minimum item
        tclosed[tile] = (f, gs) # Add to closed
        game.g.get(tile).color = Tile.TYPE_COLORS['explored'] #
        game.expanded += 1 # add one to expanded
        for neighbor in game.g.get(tile).get_walls(game.g.tx, game.g.ty): # for all actions
            
            if neighbor == target_tile:
                last = target_tile
                game.g_score[neighbor] = game.g_score[tile] + 1
                game.tree[neighbor] = tile
                break
            
            if game.g.get(neighbor).type != 'wall' and neighbor not in tclosed:
                game.g.get(neighbor).color = Tile.TYPE_COLORS['reached'] # 
            if neighbor in tclosed or game.g.get(neighbor).type == 'wall': # don't do anything if its a wall
                # game.g.get(neighbor).color = Tile.TYPE_COLORS['wall']
                continue

            # Instead of initializing them ALL at the beginning, check if it's been initialized here
            # Line 16-17 in the algorithm
            if neighbor not in game.search:
                game.search[neighbor] = 0
            
            # Line 6-8 In computePath
            if game.search[neighbor] < game.ctr:
                game.g_score[neighbor] = math.inf
                game.search[neighbor] = game.ctr
            
            # Line 9-13 in computePath
            if game.g_score[neighbor] > game.g_score[tile] + 1: # +1 represents action cost.
                game.g_score[neighbor] = game.g_score[tile] + 1
                game.tree[neighbor] = tile
                last = neighbor
                topen.push(neighbor, game.g_score[neighbor], heuristic(neighbor, target_tile)) # Line 12+13 in one (function updates automatically)
        
        # game.g.draw(s)
        # g.display.flip()
        # time.sleep(0.2)
        
    
    return topen, tclosed, last

if __name__ == "__main__":
    # add options here
    game.add_option('tw', False, DEFAULT_WIDTH, 'The number of tiles in each row of the grid')
    game.add_option('th', False, DEFAULT_HEIGHT, 'The number of tiles in each column of the grid')
    game.add_option('file', False, "", 'If specified, will read from the file instead of generating a new maze')
    game.add_option('generate', False, "", 'This will generate 50 different mazes and save them under a folder names "mazes"')
    game.add_option('b', False, "", 'Specify this option to run backwards repeated A* search')
    game.add_option('tbr', False, '', 'If specified, will break ties with the SMALLER g-score. By default breaks ties with max g_score ')
    game.run(setup, loop)
    # UNCOMMENT for getting test data from all mazes
    # for i in range(50):
    #     sys.argv = ['script', '-file' , 'mazes/maze{}.grid'.format(i), '-fps', '10000', '-b', 't']
    #     game.run(setup, loop)
        