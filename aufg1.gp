set terminal 'png'

set output 'aufg1_betrag.png'
plot 'aufg1.txt' u 3 w l title 'Betrag'


set output 'aufg1_diff.png'
plot 'aufg1.txt' u 6 w l title 'Diff'


set y2tics
set y2label "Im"
set ylabel "Re" 

stats "aufg1.txt" u 1 name "y1"
stats "aufg2.txt" u 1 name "y2"

maxabs(a,b)=(abs(a)>abs(b))*a+(abs(b)>abs(a))*b
y1r=maxabs(y1_min,y1_max)
y2r=maxabs(y2_min,y2_max)
set yrange [-y1r:y1r]
set y2range [-y2r:y2r]

set output 'aufg1_trafo.png'
plot 'aufg1.txt' u 1 axis x1y1 w l title 'Re', '' u 2 axis x1y2 w l title 'Im'


stats "aufg1.txt" u 4 name "y1"
stats "aufg2.txt" u 5 name "y2"

maxabs(a,b)=(abs(a)>abs(b))*a+(abs(b)>abs(a))*b
y1r=maxabs(y1_min,y1_max)
y2r=maxabs(y2_min,y2_max)
set yrange [-y1r:y1r]
set y2range [-y2r:y2r]



set output 'aufg1_inversetrafo.png'
plot  'aufg1.txt' u 4 w l title 'Re(Inverse)', '' u 5  axis x1y2 w l title 'Im(Inverse)'


