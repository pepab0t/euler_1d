import pandas as pd
import matplotlib.pyplot as plt


lf1 = pd.read_csv('lf1.txt', sep=';')
lf2 = pd.read_csv('lf2.txt', sep=';')
rus1 = pd.read_csv('rus1.txt', sep=';')
rus2 = pd.read_csv('rus2.txt', sep=';')
rus1_k =  pd.read_csv('rus1_konst.txt', sep=';')
rus2_k =  pd.read_csv('rus2_konst.txt', sep=';')
hll1 = pd.read_csv('hll1.txt', sep=';')
hll2 = pd.read_csv('hll2.txt', sep=';')
# ausm1 = pd.read_csv('ausm1.txt', sep=';')
# ausm2 = pd.read_csv('ausm2.txt', sep=';')
t1 = rus1['t'].iloc[0]
t2 = rus2['t'].iloc[0]
what = 'rho'

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10,7), sharex=True)

ax1.plot(lf1['x'], lf1[what], label=r'Lax-Friedrichs')
ax1.plot(rus1['x'], rus1[what], label=r'Rusanov')
ax1.plot(rus1_k['x'], rus1_k[what], label=r'Rusanov q konst.')
ax1.plot(hll1['x'], hll1[what], label=r'HLL')
# ax1.plot(ausm1['x'], ausm1[what], label=r'AUSM')
ax1.grid(True, alpha=0.5)
ax1.set_ylabel(what)
ax1.legend()
ax1.set_title(rf'$t={t1}\ s$')

ax2.plot(lf2['x'], lf2[what], label=r'Lax-Friedrichs', ls='-')
ax2.plot(rus2['x'], rus2[what], label=r'Rusanov')
ax2.plot(rus2_k['x'], rus2_k[what], label=r'Rusanov q konst.')
ax2.plot(hll2['x'], hll2[what], label=r'HLL')
# ax2.plot(ausm2['x'], ausm2[what], label=r'AUSM')
ax2.grid(True, alpha=0.5)
ax2.set_xlabel('x')
ax2.set_ylabel(what)
ax2.legend()
ax2.set_title(rf'$t={t2}\ s$')

plt.show()