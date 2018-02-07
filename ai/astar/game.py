import sys
import pygame
import time

APP_TITLE = 'Pygame test'
APP = pygame

def game(args, setup, loop):
    '''Runs the Pygame
    Arguments:
        args: the game parameters passed from the user on startup from the OPTIONS variable
        setup: A single setup function for the pygame display. Passes the screen to the function
        loop: A function which takes two arguments (delta, screen) and is run in a loop for the game.
    
    Returns:
        None: The application exits if the game loop is broken.
    '''
    pygame.init()

    size = width, height = int(args['w']), int(args['h'])

    screen = pygame.display.set_mode(size)
    max_fps = float(args['fps'])
    frame_time = 1.0 / max_fps
    delta = 1.0
    print(APP_TITLE)
    
    # User setup function
    setup(screen)

    # Event Loop
    while True:
        tick = time.time()
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()

        # User loop function
        loop(delta, screen)

        pygame.display.flip()
        tock = time.time()
        delta = tock-tick # Time to run single frame
        if delta < frame_time: time.sleep(frame_time - delta) # sleep for the rest of the duration

    sys.exit(0)
    return None        


OPTIONS = {
    'w': {
        'req': False,
        'def': 720,
        'desc': 'window height'
    },
    'h': {
        'req': False,
        'def': 480,
        'desc': 'window width'
    },
    'fps': {
        'req': False,
        'def': 30,
        'desc': 'Maximum game FPS (frames per second)'
    }
}

def add_option(name, required, default, description):
    '''Add an option that users can pass to your game at startup

    Arguments:
        name: Name of the option (users must pass a - prepended to this argument name)
        requires: boolean, whether or not the value is required
        default: What is the default value if a user doesn't pass (put None if required)
        description: Give a description of the variable for the user.
    '''
    if name in OPTIONS:
        raise ValueError("Argument name {} is already in use".format(name))
    
    OPTIONS[name] = {
        'req': required,
        'def': default,
        'desc': description
    }
    return True

def print_help():
    '''Prints the help string for the application based on the OPTIONS dictionary.
    '''
    help_str = APP_TITLE + "\n"
    for item in OPTIONS:
        help_str += "\t -{}\tRequired: {}\n\t\tDescription: {}\n".format(item,
                                                                        OPTIONS[item]['req'],
                                                                        OPTIONS[item]['desc'])
        if 'def' in OPTIONS[item]:
            help_str += '\t\tDefault:{}'.format(OPTIONS[item]['def'])
        help_str += "\n"
    print(help_str)

def parse_options(args):
    '''Parses the OPTIONS dictionary to pass argument values to the game loop
    '''
    i = 0
    opts = {}
    req_opts = filter(lambda x: OPTIONS[x]['req'] == True, OPTIONS.keys())
    while i < len(args):
        opt = args[i].strip()
        if opt[1:] not in OPTIONS:
            print("{} is not a valid parameter".format(opt))
            print_help()
            raise ValueError("Invalid argument")
        else:
            opts[opt[1:]] = args[i+1]
        i += 2
    for ri in req_opts:
        if ri not in opts:
            print("-{} is a required option.".format(ri))
            print_help()
            raise ValueError("Missing argument")
    for item in OPTIONS:
        if item not in opts:
            opts[item] = OPTIONS[item]['def']
    return opts


def run(setup, loop):
    '''Runs the game with the setup and loop functions

    Arguments:
        setup: A function with node arguments that is run once before the game loop
        loop: A function with a two arguments (delta time, game surface) that gets run up to FPS frames per second.
    '''
    args = sys.argv[1:]
    option = parse_options(args)
    game(option, setup, loop)
