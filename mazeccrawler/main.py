from random import choice

NORTH, EAST, SOUTH, WEST = 1, 2, 4, 8

TYPE_FACTORY = 0
TYPE_SCOUT = 1
TYPE_WORKER = 2
TYPE_MINER = 3


def key(col, row):
    return f"{col},{row}"


def get_wall(obs, config, col, row):
    idx = (row - obs.southBound) * config.width + col
    if 0 <= idx < len(obs.walls):
        w = obs.walls[idx]
        if w != -1:
            return w
    return 0


def can_move(obs, config, col, row, direction):
    w = get_wall(obs, config, col, row)

    if direction == "NORTH":
        return not (w & NORTH)
    if direction == "EAST":
        return not (w & EAST) and col < config.width - 1
    if direction == "SOUTH":
        return not (w & SOUTH) and row > obs.southBound
    if direction == "WEST":
        return not (w & WEST) and col > 0

    return False


def next_cell(col, row, direction):
    if direction == "NORTH":
        return col, row + 1
    if direction == "SOUTH":
        return col, row - 1
    if direction == "EAST":
        return col + 1, row
    if direction == "WEST":
        return col - 1, row
    return col, row


def safe_move(obs, config, col, row, preferred=("NORTH", "EAST", "WEST", "SOUTH")):
    for d in preferred:
        if can_move(obs, config, col, row, d):
            return d
    return "IDLE"


def find_adjacent_crystal(obs, config, col, row):
    best_dir = None
    best_energy = -1

    for d in ["NORTH", "EAST", "WEST", "SOUTH"]:
        if not can_move(obs, config, col, row, d):
            continue

        nc, nr = next_cell(col, row, d)
        crystal_energy = obs.crystals.get(key(nc, nr), 0)

        if crystal_energy > best_energy:
            best_energy = crystal_energy
            best_dir = d

    if best_dir is not None and best_energy > 0:
        return best_dir

    return None


def agent(obs, config):
    actions = {}

    my_robots = {
        uid: data
        for uid, data in obs.robots.items()
        if data[4] == obs.player
    }

    occupied_by_me = set()
    for uid, data in my_robots.items():
        _, col, row = data[0], data[1], data[2]
        occupied_by_me.add((col, row))

    for uid, data in my_robots.items():
        rtype = data[0]
        col = data[1]
        row = data[2]
        energy = data[3]
        move_cd = data[5]
        jump_cd = data[6] if len(data) > 6 else 0
        build_cd = data[7] if len(data) > 7 else 0

        w = get_wall(obs, config, col, row)

        # Emergency: if factory is close to southern boundary, move north or jump north.
        danger_distance = row - obs.southBound

        if rtype == TYPE_FACTORY:
            if danger_distance <= 2 and jump_cd == 0:
                actions[uid] = "JUMP_NORTH"
                continue

            # Build priority:
            # 1. Scout early for vision
            # 2. Worker to remove walls
            # 3. Miner if enough energy
            if build_cd == 0:
                spawn_col, spawn_row = col, row + 1
                spawn_blocked = bool(w & NORTH)

                if not spawn_blocked:
                    scout_count = sum(1 for d in my_robots.values() if d[0] == TYPE_SCOUT)
                    worker_count = sum(1 for d in my_robots.values() if d[0] == TYPE_WORKER)
                    miner_count = sum(1 for d in my_robots.values() if d[0] == TYPE_MINER)

                    if scout_count < 2 and energy >= config.scoutCost:
                        actions[uid] = "BUILD_SCOUT"
                        continue

                    if worker_count < 2 and energy >= config.workerCost:
                        actions[uid] = "BUILD_WORKER"
                        continue

                    if miner_count < 1 and energy >= config.minerCost:
                        actions[uid] = "BUILD_MINER"
                        continue

            # Move factory north if possible, otherwise sidestep.
            if move_cd == 0:
                actions[uid] = safe_move(obs, config, col, row)
            else:
                actions[uid] = "IDLE"

        elif rtype == TYPE_WORKER:
            # If north wall blocks progress, remove it.
            if energy >= config.wallRemoveCost and (w & NORTH):
                actions[uid] = "REMOVE_NORTH"
                continue

            # Collect nearby crystal if possible.
            crystal_dir = find_adjacent_crystal(obs, config, col, row)
            if crystal_dir and move_cd == 0:
                actions[uid] = crystal_dir
                continue

            if move_cd == 0:
                actions[uid] = safe_move(obs, config, col, row)
            else:
                actions[uid] = "IDLE"

        elif rtype == TYPE_MINER:
            # Transform if standing on mining node and enough energy.
            if key(col, row) in obs.miningNodes and energy >= config.transformCost:
                actions[uid] = "TRANSFORM"
                continue

            # Move toward visible mining node if nearby.
            target = None
            for pos in obs.miningNodes.keys():
                c, r = map(int, pos.split(","))
                target = (c, r)
                break

            if target and move_cd == 0:
                tc, tr = target

                if tr > row and can_move(obs, config, col, row, "NORTH"):
                    actions[uid] = "NORTH"
                elif tc > col and can_move(obs, config, col, row, "EAST"):
                    actions[uid] = "EAST"
                elif tc < col and can_move(obs, config, col, row, "WEST"):
                    actions[uid] = "WEST"
                else:
                    actions[uid] = safe_move(obs, config, col, row)
                continue

            crystal_dir = find_adjacent_crystal(obs, config, col, row)
            if crystal_dir and move_cd == 0:
                actions[uid] = crystal_dir
                continue

            if move_cd == 0:
                actions[uid] = safe_move(obs, config, col, row)
            else:
                actions[uid] = "IDLE"

        elif rtype == TYPE_SCOUT:
            # Scout explores fast and collects crystals.
            crystal_dir = find_adjacent_crystal(obs, config, col, row)
            if crystal_dir and move_cd == 0:
                actions[uid] = crystal_dir
                continue

            if move_cd == 0:
                # Prefer north, then side movement for exploration.
                actions[uid] = safe_move(obs, config, col, row, preferred=("NORTH", "EAST", "WEST", "SOUTH"))
            else:
                actions[uid] = "IDLE"

    return actions