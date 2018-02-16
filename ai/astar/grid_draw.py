import game
from game import APP as g
from game_objects import Grid, Tile

DEFAULT_WIDTH = 101
DEFAULT_HEIGHT = 101

def maze_view_setup(screen, args):
    game.g = Grid.gen_grid(0, 0, int(args['w']), int(args['h']), int(args['tw']), int(args['th']))
    for row in game.g.matrix:
            for col in row:
                col.update(tt='wall')
    game.start = (1, 1)
    game.g.get(game.start).update(tt='path')
    game.g.get(game.start).color = Tile.TYPE_COLORS['agent']
    game.dir_stack = []
    game.curr_tile = game.start
    game.dirs = game.g.gen_rand_dirs()
    for d in game.dirs:
        game.dir_stack.append((game.curr_tile, d))
    # game.g.dfs_maze()

def maze_view_LOOP(d, s):

    if len(game.dir_stack) > 0:
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
    else:
        pass
    game.g.draw(s)


if __name__ == "__main__":
    # add options here
    game.add_option('tw', False, DEFAULT_WIDTH, 'The number of tiles in each row of the grid')
    game.add_option('th', False, DEFAULT_HEIGHT, 'The number of tiles in each column of the grid')
    game.run(maze_view_setup, maze_view_LOOP)

