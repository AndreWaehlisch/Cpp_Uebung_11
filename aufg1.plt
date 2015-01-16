set terminal 'png'

set output 'aufg1_reim.png'
plot 'aufg1.txt' u 1 w l title 'Re', '' u 2 w l title 'Im'

set output 'aufg1_re.png'
plot 'aufg1.txt' u 1 w l title 'Realteil'

set output 'aufg1_im.png'
plot 'aufg1.txt' u 2 w l title 'Imaginärteil'

set output 'aufg1_betrag.png'
plot 'aufg1.txt' u 3 w l title 'Betrag'

set output 'aufg1_input.png'
plot 'aufg1_input.txt' u 1 w l title 'Input data'

set output 'aufg1_inverse.png'
plot 'aufg1.txt' u 4 w l title 'Re(Inverse)', '' u 5 w l title 'Im(Inverse)'

set output 'aufg1_diff.png'
plot 'aufg1.txt' u 6 w l title 'Diff'
