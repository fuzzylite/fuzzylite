library('cowplot')
library('ggplot2')

theme_set(theme_gray())


isEq = function(a,b,error=1e-5){
    abs(a - b) <= error
}

isLt = function(a,b,error=1e-5){
    !isEq(a,b,error) & a < b
}

isGt = function(a,b,error=1e-5){
    !isEq(a,b,error) & a > b
}

isLE = function(a,b,error=1e-5){
    isEq(a,b,error) | a < b
}

isGE = function(a,b,error=1e-5){
    isEq(a,b,error) | a > b
}

#Scale: Scales a number to a different scale
Scale = function(value, toMin, toMax, fromMin=min(value), fromMax=max(value)){
      (toMax - toMin) / (fromMax - fromMin) * (value - fromMin) + toMin
}

#Basic

term.triangle = function(x, a, b, c){
    if (isLt(x,a) | isGt(x,c)) 
        0.0
    else if (isEq(x,b))
        1.0
    else if (isLt(x,b))
        (x - a) / (b - a)
    else 
        result = (c - x) / (c - b)    
}

term.trapezoid = function(x,a,b,c,d){
        if (isLt(x, a) | isGt(x, d))
            0.0
        else if (isLE(x, b))
            min(1.0, (x - a) / (b - a))
        else if (isLE(x, c))
            1.0
        else if (isLE(x, d))
            (d - x) / (d - c)
}

term.rectangle = function(x,a,b){
    if (isLt(x, a) | isGt(x,b)) 
        0.0
    else 1.0
}

term.discrete = function(x, a, b){
    diff = b-a
    if (isLt(x,a) | isGt(x,b))
        0.0
    else if (isLt(x,.25 * diff))
        Scale(x, 0, 1, a, a + diff*.25)
    else if (isLt(x, .5*diff))
        Scale(x,1,0.5, a + diff*.25, a + diff*.5)
    else if (isLt(x, .75*diff))
        Scale(x,0.5,1,a+diff*.5, a+diff*.75)
    else if (isLt(x, diff))
        Scale(x, 1, 0, a+diff*.75, a+diff)
    else {0.0}
#    else if (isLt(x, a*.5)
}


term.bell = function(x, center, width, slope){
    1.0 / (1.0 + abs((x - center) / width)**(2 * slope));
}

#EXTENDED

term.cosine = function(x, center, width){
    if (isLt(x, center-width/2.0) || isGt(x, center+width/2.0)){
        0.0
    }else{
        0.5 * (1.0 + cos(2.0 / width * pi * (x - center)))
    }
}

term.gaussian = function(x, mean, sd){
    exp((-(x - mean) * (x - mean)) / (2 * sd * sd));
}

term.gaussianProduct = function(x, meanA, sdA, meanB, sdB){
        xLEa = isLE(x, meanA)
        a = exp((-(x - meanA) * (x - meanA)) / (2 * sdA * sdA)) * xLEa + (1 - xLEa)
        xGEb = isGE(x, meanB)
        b = exp((-(x - meanB) * (x - meanB)) / (2 * sdB * sdB)) * xGEb + (1 - xGEb)
        a * b
}

term.bell = function(x, center, width, slope){
    1.0 / (1.0 + abs((x - center) / width)** (2 * slope))
}

term.piShape = function(x, bottomLeft, topLeft, topRight, bottomRight){
        a_b_ave = (bottomLeft + topLeft) / 2.0
        b_minus_a = topLeft - bottomLeft
        c_d_ave = (topRight + bottomRight) / 2.0
        d_minus_c = bottomRight - topRight

        if (isLE(x, bottomLeft)) 0.0

        else if (isLE(x, a_b_ave))
            2.0 * ((x - bottomLeft) / b_minus_a) ** 2

        else if (isLt(x, topLeft))
            1.0 - 2.0 * ((x - topLeft) / b_minus_a)** 2

        else if (isLE(x, topRight))
            1.0

        else if (isLE(x, c_d_ave))
            1 - 2 * ((x - topRight) / d_minus_c)**2

        else if (isLt(x, bottomRight))
            2 * ((x - bottomRight) / d_minus_c)** 2

        else 0.0
}

term.sigmoidDifference = function(x, left, rising, falling, right){
        a = 1.0 / (1 + exp(-rising * (x - left)))
        b = 1.0 / (1 + exp(-falling * (x - right)))
        abs(a - b)
}

term.sigmoidProduct = function(x, left, rising, falling, right){
        a = 1.0 / (1 + exp(-rising * (x - left)))
        b = 1.0 / (1 + exp(-falling * (x - right)))
        abs(a * b)
}

term.spike = function(x, center, width){
    exp(-abs( 10 / width * (x-center)))
}

#EDGES

term.binary = function(x, threshold, direction){
    if (isGE(direction,0.0) && isGE(x, threshold)){
        1.0
    }else if (isLt(direction, 0.0) && isLE(x, threshold)){
        1.0
    }else{
        0.0
    }
}

term.concave = function(x, inflection, end){
    if (isLE(inflection, end)){ #Concave increasing
        if (isLt(x, end)){
            (end - inflection) / (2 * end - inflection - x)
        }else{ 1.0 }
    } else{ #Concave decreasing
        if (isGt(x, end)){
            (inflection - end) / (inflection -2 * end + x)
        }else{ 1.0 }
    }
}


term.ramp = function(x, start, end){
        if (isEq(start, end)) 0.0
        
        else if (isLt(start, end)) {
            if (isLE(x, start)) 0.0
            else if (isGE(x, end))  1.0
            else (x - start) / (end - start)
        } else {
            if (isGE(x, start)) 0.0
            else if (isLE(x, end)) 1.0
            else (start - x) / ( start - end)
        }
}

term.sigmoid = function(x, inflection, slope){
    1.0 / (1.0 + exp(-slope * (x - inflection)))
}

term.sShape = function(x, start, end){
        average = (start + end) / 2
        difference = end - start

        if (isLE(x, start)) 0.0

        else if (isLE(x, average))
            2 * ((x - start) / difference) ** 2

        else if (isLt(x, end))
            1.0 - 2.0 * ((x - end) / difference) ** 2
        else 1.0
}

term.zShape = function(x, start, end){
        average = (start + end) / 2
        difference = end - start

        if (isLE(x, start)) 1.0

        else if (isLE(x, average))
            1.0 - 2.0 * ((x - start) / difference) ** 2

        else if (isLt(x, end))
            2.0 * ((x - end) / difference)** 2

        else 0.0
}







x = seq(0,1,length=500)
average = (max(x) + min(x))/2
diff = (max(x) - min(x))





dir.create('figure',showWarnings=F)


##################BASIC

triangle.df = data.frame(x, y=sapply(x, term.triangle, min(x), average, max(x)))
triangle.plot = ggplot(triangle.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/triangle.svg', triangle.plot, width=3, height=3)


trapezoid.df = data.frame(x, y=sapply(x, term.trapezoid, min(x), min(x) + .25 * diff, min(x) + .75*diff, max(x)))
trapezoid.plot = ggplot(trapezoid.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/trapezoid.svg', trapezoid.plot, width=3, height=3)


rectangle.df = data.frame(x, y=sapply(x, term.rectangle, min(x) + .25*diff,  min(x) + .75*diff))
rectangle.plot = ggplot(rectangle.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/rectangle.svg', rectangle.plot, width=3, height=3)


discrete.x = x[c(1, seq(10, 495, 10), 500)]
discrete.df = data.frame(x=discrete.x, y=sapply(discrete.x, term.discrete, min(x),  max(x)))
discrete.plot = ggplot(discrete.df, aes(x,y, size=2, lineend='round')) + geom_point() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/discrete.svg', discrete.plot, width=3, height=3)



#################EXTENDED

cosine.df = data.frame(x, y=sapply(x, term.cosine, average, diff))
cosine.plot = ggplot(cosine.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/cosine.svg', cosine.plot, width=3, height=3)


gaussian.df = data.frame(x, y=sapply(x, term.gaussian, average,  .2*diff))
gaussian.plot = ggplot(gaussian.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/gaussian.svg', gaussian.plot, width=3, height=3)


gaussianProduct.df = data.frame(x, y=sapply(x, term.gaussianProduct, average+.1,  .2*diff, average-.1, .2*diff))
gaussianProduct.plot = ggplot(gaussianProduct.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/gaussianProduct.svg', gaussianProduct.plot, width=3, height=3)


bell.df = data.frame(x, y=sapply(x, term.bell, average,  .25*diff, 3.0))
bell.plot = ggplot(bell.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/bell.svg', bell.plot, width=3, height=3)


piShape.df = data.frame(x, y=sapply(x, term.piShape, min(x), average, average, max(x)))
piShape.plot = ggplot(piShape.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/piShape.svg', piShape.plot, width=3, height=3)


sigmoidDifference.df = data.frame(x, y=sapply(x, term.sigmoidDifference, min(x) + .25*diff, 20/diff, 20/diff, min(x)+.75*diff))
sigmoidDifference.plot = ggplot(sigmoidDifference.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/sigmoidDifference.svg', sigmoidDifference.plot, width=3, height=3)


sigmoidProduct.df = data.frame(x, y=sapply(x, term.sigmoidProduct, min(x) + .25*diff, 10/diff, -10/diff, min(x)+.75*diff))
sigmoidProduct.plot = ggplot(sigmoidProduct.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/sigmoidProduct.svg', sigmoidProduct.plot, width=3, height=3)


spike.df = data.frame(x, y=sapply(x, term.spike, average, diff))
spike.plot = ggplot(spike.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/spike.svg', spike.plot, width=3, height=3)



###############EDGE

#Binary
binary.dfa = data.frame(x, y=sapply(x, term.binary, min(x) + .25*diff, -1))
binary.dfb = data.frame(x, y=sapply(x, term.binary, min(x) + .75*diff, 1))
binary.plot = ggplot(binary.dfa, aes(x,y, size=2, lineend='round')) + 
        geom_line(data=binary.dfb, aes(x,y, size=2, lineend='round')) + 
        geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')   
ggsave('figure/binary.svg', binary.plot, width=3, height=3)

#Concave
concave.dfa = data.frame(x, y=sapply(x, term.concave, average, max(x)-.2))
concave.dfb = data.frame(x, y=sapply(x, term.concave, average, 1-(max(x)-.2)))
concave.plot = ggplot(concave.dfa, aes(x,y, size=2, lineend='round')) + 
        geom_line(data=concave.dfb, aes(x,y, size=2, lineend='round')) + 
        geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')   
ggsave('figure/concave.svg', concave.plot, width=3, height=3)

#RAMP
ramp.dfa = data.frame(x, y=sapply(x, term.ramp, max(x)-.2, min(x)+.2))
ramp.dfb = data.frame(x, y=sapply(x, term.ramp, min(x)+.2, max(x)-.2))
ramp.plot = ggplot(ramp.dfa, aes(x,y, size=2, lineend='round')) + 
        geom_line(data=ramp.dfb, aes(x,y, size=2, lineend='round')) + 
        geom_line() + ylab(expression(mu(x))) + theme(legend.position='none') 
ggsave('figure/ramp.svg', ramp.plot, width=3, height=3)


sigmoid.dfa = data.frame(x, y=sapply(x, term.sigmoid, average, 20/diff))
sigmoid.dfb = data.frame(x, y=sapply(x, term.sigmoid, average, -20/diff))
sigmoid.plot = ggplot(sigmoid.dfa, aes(x,y, size=2, lineend='round')) + 
        geom_line(data=sigmoid.dfb, aes(x,y, size=2, lineend='round')) + 
        geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/sigmoid.svg', sigmoid.plot, width=3, height=3)


sShape.df = data.frame(x, y=sapply(x, term.sShape, min(x), max(x)))
sShape.plot = ggplot(sShape.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/sShape.svg', sShape.plot, width=3, height=3)


zShape.df = data.frame(x, y=sapply(x, term.zShape, min(x), max(x)))
zShape.plot = ggplot(zShape.df, aes(x,y, size=2, lineend='round')) + geom_line() + ylab(expression(mu(x))) + theme(legend.position='none')
ggsave('figure/zShape.svg', zShape.plot, width=3, height=3)


##############Terms

    constant.plot = 
         ggplot(data=data.frame(x=0,y=0)) +
         geom_point(aes(x=0,y=0), size=0) +
         ylab(expression(f(x))) + xlab('x') + 
         coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
         annotate('text', x = .5, y = .5, label = "f(x)==k", parse = T,size=10) 
     
    linear.plot = 
         ggplot(data=data.frame(x=0,y=0)) + 
         geom_point(aes(x=0,y=0), size=0) +
         ylab(expression(f(x))) + xlab('x') + 
         coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
         annotate('text', x = .5, y = .5, angle=45, label = "f(x)==sum(paste(c[i],v[i]), i)+k", parse = T,size=10)
             
    function.plot = 
        ggplot(data=data.frame(x=0,y=0)) + 
        geom_point(aes(x=0,y=0), size=0) +
        ylab(expression(f(x))) + xlab('x') +
        coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
        annotate('text', x = .5, y = .5, label = "f:x %->% f(x)", parse = T,size=10)


########## Vertical layout
        terms.grid = plot_grid(
            
triangle.plot,      bell.plot,                piShape.plot,
trapezoid.plot,     cosine.plot,              sigmoidDifference.plot,
rectangle.plot,     gaussian.plot,            sigmoidProduct.plot,
discrete.plot,      gaussianProduct.plot,     spike.plot,
binary.plot,        ramp.plot,                sShape.plot,
concave.plot,       sigmoid.plot,             zShape.plot, 
function.plot,      linear.plot,              constant.plot,

        ncol=3, nrow=7, scale=1, label_size=12, vjust=1.25, align='v',
        #hjust=.5, # align='hv',

    labels=c(
'Triangle',   'Bell',             'PiShape',
'Trapezoid',  'Cosine',           'SigmoidDifference',
'Rectangle',  'Gaussian',         'SigmoidProduct',
'Discrete',   'GaussianProduct',  'Spike',
'Binary',     'Ramp',             'SShape',
'Concave',    'Sigmoid',          'ZShape',
'Function',   'Linear',           'Constant'
    )
    ) 
    
    save_plot('figure/terms.svg', terms.grid, ncol=3, nrow=7, scale=.75)

    stop('Script successfully executed')


##########Horizontal layout
    terms.grid = plot_grid(

    triangle.plot, trapezoid.plot, rectangle.plot, discrete.plot, function.plot,  binary.plot , concave.plot, 

    bell.plot, cosine.plot, gaussian.plot, gaussianProduct.plot,  linear.plot, ramp.plot, sigmoid.plot,

    piShape.plot, sigmoidDifference.plot,  sigmoidProduct.plot, spike.plot, constant.plot, sShape.plot, zShape.plot,

    ncol=7, nrow=3, scale=1, label_size=12, vjust=1.25, align='v',
    #hjust=.5, # align='hv',

labels=c(
'Triangle', 'Trapezoid', 'Rectangle', 'Discrete', 'Function', 'Binary',  'Concave',
'Bell', 'Cosine', 'Gaussian', 'GaussianProduct', 'Linear',  'Ramp', 'Sigmoid',
'PiShape', 'SigmoidDifference', 'SigmoidProduct','Spike', 'Constant', 'SShape', 'ZShape')
) 
    
save_plot('figure/terms.svg', terms.grid, ncol=7, nrow=3, scale=.75)

stop('Script successfully executed')












# X1234X
# 123456
# 7    8
# 123456

constant.plot = 
     ggplot(data=data.frame(x=0,y=0)) +
     geom_point(aes(x=0,y=0), size=0) +
     ylab(expression(mu(x))) + xlab('x') + 
     coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
     annotate('text', x = .5, y = .5, label = "mu(x)==k", parse = T,size=10) 

linear.plot = 
     ggplot(data=data.frame(x=0,y=0)) + 
     geom_point(aes(x=0,y=0), size=0) +
     ylab(expression(mu(x))) + xlab('x') + 
     coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
     annotate('text', x = .5, y = .5, angle=45, label = "mu(x)==sum(c[i] %.% x[i], i)+k", parse = T,size=5)
     
function.plot = 
    ggplot(data=data.frame(x=0,y=0)) + 
    geom_point(aes(x=0,y=0), size=0) +
    ylab(expression(mu(x))) + xlab('x') +
    coord_cartesian(ylim = c(-0.05, 1.05), xlim=c(-0.05,1.05)) + 
    annotate('text', x = .5, y = .5, label = "f:x %->% mu(x)", parse = T,size=5)


terms.grid = plot_grid(

NULL, triangle.plot, trapezoid.plot, rectangle.plot, discrete.plot, NULL,

bell.plot, cosine.plot, gaussian.plot, gaussianProduct.plot,  piShape.plot, sigmoidDifference.plot, 

sigmoidProduct.plot, spike.plot, NULL, constant.plot, linear.plot, function.plot,  

binary.plot, concave.plot, ramp.plot, sigmoid.plot, sShape.plot, zShape.plot,

nrow=4, ncol=6, scale=1, label_size=12, vjust=1.25,
#hjust=.5, # align='hv',

labels=c(
'', 'Triangle', 'Trapezoid', 'Rectangle', 'Discrete', '',
'Bell', 'Cosine', 'Gaussian', 'GaussianProduct', 'PiShape', 'SigmoidDifference',
'SigmoidProduct','Spike', '', 'Constant', 'Linear', 'Function',
'Binary',  'Concave', 'Ramp', 'Sigmoid', 'SShape', 'ZShape')
) 

save_plot('figure/terms.svg', terms.grid,
    ncol=6, nrow=4, scale=.5)




