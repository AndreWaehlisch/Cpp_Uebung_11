set terminal pngcairo
set output "aufg4_abw.png"
set xlabel "k" 
plot "aufg4_abw.txt" w l t "g_interp - g_analytisch"
set output "aufg4_interp.png"
set y2tics 
set ylabel "Re"
set y2label "Im"
plot "aufg4_interp.txt" u 1:2 axis x1y1 w l t "Re(g)", '' u 1:3 axis x1y2 w l t "Im (g)"
