set terminal 'png'
maxabs(a,b)=abs((abs(a)>abs(b))*a+(abs(b)>abs(a))*b)

set output 'aufg2_betrag.png'
plot 'aufg2.txt' u 3 w l title 'Betrag'


set output 'aufg2_diff.png'
plot 'aufg2.txt' u 6 w l title 'Diff'


set y2tics
set y2label "Im"
set ylabel "Re" 

stats "aufg2.txt" u 1 name "y1"
stats "aufg2.txt" u 2 name "y2"


y1r=maxabs(y1_min,y1_max)
y2r=maxabs(y2_min,y2_max)
set yrange [-y1r:y1r]
set y2range [-y2r-1:y2r+1]

set output 'aufg2_trafo.png'
plot 'aufg2.txt' u 1 axis x1y1 w l title 'Re', '' u 2 axis x1y2 w l title 'Im'


stats "aufg2.txt" u 4 name "y1"
stats "aufg2.txt" u 5 name "y2"


y1r=maxabs(y1_min,y1_max)
y2r=maxabs(y2_min,y2_max)
set yrange [-y1r:y1r]
set y2range [-y2r:y2r]



set output 'aufg2_inversetrafo.png'
plot  'aufg2.txt' u 4 w l title 'Re(Inverse)', '' u 5  axis x1y2 w l title 'Im(Inverse)'


