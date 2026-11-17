# Question 1: Blocks World - Goal Stack Planning
# Initial state: A, B, C and D are on the table.
# Goal state: A is on C and D is on B.

state = {
    "OnTable(A)", "OnTable(B)", "OnTable(C)", "OnTable(D)",
    "Clear(A)", "Clear(B)", "Clear(C)", "Clear(D)",
    "ArmEmpty"
}

def show_state(title):
    print("\n" + title)
    for item in sorted(state):
        print(item)

def pickup(block):
    required = {
        f"OnTable({block})",
        f"Clear({block})",
        "ArmEmpty"
    }

    if not required.issubset(state):
        raise ValueError(f"Cannot PICKUP({block}): preconditions not satisfied.")

    state.remove(f"OnTable({block})")
    state.remove(f"Clear({block})")
    state.remove("ArmEmpty")
    state.add(f"Holding({block})")
    print(f"Action: PICKUP({block})")

def stack(block, destination):
    required = {
        f"Holding({block})",
        f"Clear({destination})"
    }

    if not required.issubset(state):
        raise ValueError(
            f"Cannot STACK({block}, {destination}): preconditions not satisfied."
        )

    state.remove(f"Holding({block})")
    state.remove(f"Clear({destination})")
    state.add(f"On({block},{destination})")
    state.add(f"Clear({block})")
    state.add("ArmEmpty")
    print(f"Action: STACK({block}, {destination})")

def main():
    show_state("Initial State:")

    print("\nGoal Stack / Action Sequence:")
    pickup("A")
    stack("A", "C")
    pickup("D")
    stack("D", "B")

    show_state("Final State:")

    goals = {"On(A,C)", "On(D,B)"}
    if goals.issubset(state):
        print("\nBoth goal conditions have been achieved.")
    else:
        print("\nGoal state not achieved.")

if __name__ == "__main__":
    main()
