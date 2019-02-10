set terminal png

do for [t in "10 50 100"] {
  set output 'mklVSopenblas'.t.'.png'

  set style fill solid 0.5
  set style data boxplot
  set boxwidth  0.5
  set pointsize 0.5
  
  set border 2
  set xtics ("mkl" 1, "openblas" 2) 
  set yrange [0:600]

  plot for [i=1:2] './timefiles/combined'.t.'.txt' using (i):i notitle
}
do for [t in "500 1000"] {
  set output 'mklVSopenblas'.t.'.png'

  set style fill solid 0.5
  set style data boxplot
  set boxwidth  0.5
  set pointsize 0.5
  
  set border 2
  set xtics ("mkl" 1, "openblas" 2) 
  set yrange [500:6000]

  plot for [i=1:2] './timefiles/combined'.t.'.txt' using (i):i notitle
}

do for [t in "10 50 100"] {
  set output 'iterationVSpthread'.t.'.png'

  set style fill solid 0.5
  set style data boxplot
  set boxwidth  0.5
  set pointsize 0.5
  
  set border 2
  set xtics ("pthread" 1, "iteration" 2) 
  set yrange [1000:20000]

  plot for [i=3:4] './timefiles/combined'.t.'.txt' using (i):i notitle
}
do for [t in "500 1000"] {
  set output 'iterationVSpthread'.t.'.png'

  set style fill solid 0.5
  set style data boxplot
  set boxwidth  0.5
  set pointsize 0.5
  
  set border 2
  set xtics ("mkl" 1, "openblas" 2) 
  set yrange [30000:200000]

  plot for [i=3:4] './timefiles/combined'.t.'.txt' using (i):i notitle
}


