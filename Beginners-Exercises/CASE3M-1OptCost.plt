set title 'CASE3M-1OptCost' noenhanced
set autoscale
set terminal wxt
set size 1.0, 1.0
set palette model RGB defined(0 "green", 1 "blue", 2 "red")
set grid xtics
set grid ytics
set xlabel "Total Cost GBP" offset 0, 0.5
set ylabel "(Target - Simulated) / Target" offset 1.5, 0
set cblabel "Total Injected Water bbls" offset -0.5, 0
plot 'F:\Oscar\Old E\Old F\Squeeze VI Versions\2017\SQUEEZE 10 04.2017 Bridging Adiditive_PSO ID Parallel\Beginners-Exercises\CASE3M-1OptPSOAll.txt' using 15:16:12 title 'Squeeze Designs' with points pt 7 lc palette, \
'F:\Oscar\Old E\Old F\Squeeze VI Versions\2017\SQUEEZE 10 04.2017 Bridging Adiditive_PSO ID Parallel\Beginners-Exercises\CASE3M-1OptPSOCostPar.txt' using 11:12:8 with  p pt 7 palette notitle, \
'F:\Oscar\Old E\Old F\Squeeze VI Versions\2017\SQUEEZE 10 04.2017 Bridging Adiditive_PSO ID Parallel\Beginners-Exercises\CASE3M-1OptPSOCostPar.txt' using 11:12 title 'Pareto Front' with l lc - 1