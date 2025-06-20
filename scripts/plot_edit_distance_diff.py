import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys

# Lê o argumento passado pela linha de comando
if len(sys.argv) < 2:
    print("Uso: python3 plot_edit_distance_diff.py <TreeType>")
    sys.exit(1)

tree_type = sys.argv[1]
file_name = f"{tree_type}_data.csv"
plots_path = "results/plots"
raw_path = "results/raw"

# Carrega o CSV
df = pd.read_csv(f"{raw_path}/{file_name}")

tree_type = tree_type.lower()

# Calcula diferença absoluta da distância de edição
df["edit_distance_diff"] = (df["edit_distance_t1"] - df["edit_distance_t2"]).abs()

# Gera pivot table para o heatmap
pivot = df.pivot(index="T1", columns="T2", values="edit_distance_diff")

# Plota o heatmap
plt.figure(figsize=(10, 6))
sns.heatmap(pivot, annot=True, fmt=".0f", cmap="Reds", cbar_kws={"label": "Diferença |d1 - d2|"})
plt.title(f"Diferença absoluta da distância de edição ({tree_type} Tree)")
plt.xlabel("Tamanho de T2")
plt.ylabel("Tamanho de T1")
plt.tight_layout()
plt.savefig(f"{plots_path}/heatmap_diff_edit_distance_{tree_type}_tree.png")
# plt.show()
