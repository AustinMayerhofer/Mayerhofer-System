# %%
import pandas as pd
import matplotlib.pyplot as plt

# %%
df = pd.read_csv('ranks.csv')
top_25 = df.loc[(df['rank'] >= 1) & (df['rank'] <= 25)]

# %%
# https://www.machinelearningplus.com/plots/top-50-matplotlib-visualizations-the-master-plots-python/#16.-Lollipop-Chart
# Draw plot
fig, ax = plt.subplots(figsize=(16,10), dpi= 80)
ax.vlines(x=top_25['rank'], ymin=0, ymax=top_25['ranking_points'], color='firebrick', alpha=0.7, linewidth=2)
ax.scatter(x=top_25['rank'], y=top_25['ranking_points'], s=15, color='firebrick', alpha=0.7)

# Title, Label, Ticks and Ylim
ax.set_title('Top 25', fontdict={'size':22})
ax.set_ylabel('Resume points')
ax.set_xticks(top_25['rank'])
ax.set_xticklabels(top_25['name'], rotation=60, fontdict={'horizontalalignment': 'right', 'size':12})
ax.set_ylim(0, top_25['ranking_points'].max() + 5)

# Annotate
for row in top_25.itertuples():
    ax.text(row.rank, row.ranking_points, s=round(row.ranking_points, 2), horizontalalignment= 'center', verticalalignment='bottom', fontsize=11)

plt.show()