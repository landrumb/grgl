# %%
# %pip install pyigd
from pygrgl import GRG, load_immutable_grg, get_dfs_order, TraversalDirection, get_topo_order
from pygrgl.display import grg_to_cyto
from IPython.display import display, SVG
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os, sys
import pyigd
from collections import defaultdict
import torch

# %%
igd_file = "/global/homes/l/landrum/data/Filtered_Genotype_Data_2022.igd"
igd = pyigd.IGDFile(igd_file)

index_to_pedigree = igd.get_individual_ids()
pedigree_to_index = {pedigree: index for index, pedigree in enumerate(index_to_pedigree)}
len(pedigree_to_index)

# %%
# import the latent phenotypes
labels = pd.read_csv('~/data/labels_latentspace32_2022.csv')
labels

# %%


# %%
grg = load_immutable_grg('/global/homes/l/landrum/data/corn.igd.final.grg')
print(f"nodes: {grg.num_nodes:,}, edges: {grg.num_edges:,}, samples: {grg.num_samples:,}")
# display(grg_to_cyto(grg))

# %%
train_pedigrees = labels[labels['train_set'] == True]['Pedigree'].unique()
len(train_pedigrees)

# %%
train_embeddings_mean = np.zeros((len(train_pedigrees), 32), dtype=np.float32)
for i, pedigree in enumerate(train_pedigrees):
    for j in range(32):
        train_embeddings_mean[i, j] = labels[labels['Pedigree'] == pedigree][f"V{j+1}"].mean()
        
train_embeddings_mean

# %%
embedding_centroid = np.mean(train_embeddings_mean, axis=0)
embedding_centroid

# %%
normalized = train_embeddings_mean - embedding_centroid
normalized

# %%
node_vectors = torch.randn((grg.num_nodes, 32), requires_grad=True) / 50000

print(f"initial weights example: {node_vectors[0]}")

# %%
# define the loss function

