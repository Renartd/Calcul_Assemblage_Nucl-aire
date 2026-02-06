from src.core_q_learning import simulated_annealing
from src.core_config import power_field

best_config, best_R = simulated_annealing(episodes=300, steps=40)

print("Meilleure récompense (−variance de puissance) :", best_R)
P = power_field(best_config)

for row_cfg, row_p in zip(best_config, P):
    line_cfg = " ".join(c if c is not None else '_' for c in row_cfg)
    line_p = " ".join(f"{v:.2f}" if v > 0 else "__" for v in row_p)
    print(line_cfg, "   ", line_p)
