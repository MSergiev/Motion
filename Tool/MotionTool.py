#!/usr/bin/python

from Tkinter import *
from PIL import Image, ImageTk
import time
from subprocess import call

fps_del = 1000/60

root = Tk()
root.title( "Motion Tool" )

selected = 0
accel = 0
type = 0
direction = 0
text = "Select function or enter config parameters"
graphType = 0
graphW = 300
graphH = 330
animW = 300
animH = 300
    
filename = 'motion_plot.xls'

examples = [ 'linear', 
             'pow', 
             'quad', 
             'cubic', 
             'back', 
             'circular', 
             'elastic', 
             'bounce', 
             'sbounce', 
             'sine', 
             'exp'
            ]
example_dir = [ 'in', 'out', 'both' ]


# Call interpolator

def Interpolate():
    file = 'examples/' + examples[selected] + '_' + example_dir[accel] + '.txt'
    SetText( file )
    call(["./MotionTool", file])
    ReadFile()
  
# Read values file

currentValue = 0
interpolatedValues = []
graphValues = []
currentLine = 0
currentIntersect = 0

def ReadFile():
    
    global interpolatedValues
    global currentValue
    
    currentValue = 0
    
    with open( filename ) as f:
        interpolatedValues = f.readlines()
        interpolatedValues = [float(x.strip()) for x in interpolatedValues] 
    
    Plot()
    
# Graph

topf = Frame( root )
topf.pack( side = TOP )

w = Canvas( topf, width=graphW, height=graphH, bg="white" )
w.pack( side = LEFT )

lineOff = 12
startX = lineOff
startY = graphH-lineOff
endX = graphW-lineOff
endY = lineOff

# Labels
w.create_text( graphW-4, graphH-4, text="t")
w.create_text( 6, graphH-4, text="O")
w.create_text( graphW-4, 8, text="1")


# Plot data
axes = []
grid = []
lines = []
yLabel = w.create_text( 4, 8, text="S")
max = -float("inf")
min = float("inf")
graphMax = -float("inf")
graphMin = float("inf")

def Plot():
    
    global graphValues
    global axes
    global grid
    global lines
    global yLabel
    global max
    global min
    global graphMax
    global graphMin
    

    for i in axes:
        w.delete( i )
    
    for i in grid:
        w.delete( i )
        
    for i in lines:
        w.delete( i )
        
    w.delete( yLabel )
    
    yLabelText = 'S'
    graphColor = 'red'
    
    if graphType == 1:
        yLabelText = 'v'
        graphColor = 'blue'
    elif graphType == 2:
        yLabelText = 'a'
        graphColor = 'green'
    
    axes = []
    grid = []
    lines = []
    
    max = -float("inf")
    min = float("inf")
    
    for i in range( 0, len(interpolatedValues) ):
        if interpolatedValues[i] > max : max = interpolatedValues[i]
        if interpolatedValues[i] < min : min = interpolatedValues[i]
    
    graphValues = []
    for i in range( 1, len(interpolatedValues)-1 ):
        
        if graphType == 0:
            graphValues.append( interpolatedValues[i])
        elif graphType == 1:
            graphValues.append( interpolatedValues[i] - interpolatedValues[i-1] )
        else:
            graphValues.append( (interpolatedValues[i+1] - interpolatedValues[i]) - (interpolatedValues[i] - interpolatedValues[i-1]) )
            
        if i == 1:
            graphValues.append(graphValues[-1])
    graphValues.append(graphValues[-1])
    
    length = len(graphValues)
    
    graphMax = -float("inf")
    graphMin = float("inf")
    
    for i in range( 0, length ):
        if graphValues[i] > graphMax : graphMax = graphValues[i]
        if graphValues[i] < graphMin : graphMin = graphValues[i]
    
    xWidth = (endX-startX)
    yWidth = (graphMax-graphMin)
    
    # Plot graph values
    for i in range( 1, length ):
        
        xVal = 0
        if length < 0.00001:
            xVal = i * xWidth + startX
        else:
            xVal = float(i)/length * xWidth + startX
        
        yVal = 0
        if yWidth < 0.00001:
            yVal = graphValues[i] * (endY-startY) + startY
        else:
            yVal = (graphValues[i]-graphMin)/yWidth * (endY-startY) + startY
            
        lines.append( w.create_line( xVal, yVal, xVal, startY, fill=graphColor, width=2 ) )
    

    # End lines 
    grid.append( w.create_line( startX, endY, endX, endY, fill="gray", dash=(4, 4) ) )
    grid.append( w.create_line( endX, startY, endX, endY, fill="gray", dash=(4, 4) ) )
    # Mid lines
    grid.append( w.create_line( lineOff, graphH/2, graphW-lineOff, graphH/2, fill="gray", dash=(4, 4) ) )
    grid.append( w.create_line( graphW/2, graphH-lineOff, graphW/2, lineOff, fill="gray", dash=(4, 4) ) )   
    # Main axes
    axes.append( w.create_line( startX, endY-8, startX, startY+2, fill="black", width=3, arrow=FIRST ) )
    axes.append( w.create_line( endX+8, startY, startX, startY, fill="black", width=3, arrow=FIRST ) )
    
    # Y label
    yLabel = w.create_text( 4, 8, text=yLabelText)
    


# Animation

af = Frame( topf )
af.pack( side = LEFT )

w2 = Canvas( af, width=animW, height=animH, bg="white" )
w2.pack( side = TOP )

#w2.create_line( 0, animH, animW, 0, fill="black", dash=(4, 4) )
#w2.create_line( 0, 0, animW, animH, fill="black", dash=(4, 4) )

play = False
def Play(): global play; play = not play
playButton = Button( af, text="Play", command=Play )
playButton.pack( fill=X )


# Easing function buttons

bf = Frame( topf )
bf.pack( side = LEFT )

funcLabel = Label( bf )
funcLabel.config( text = "Functions" )
funcLabel.pack( side = TOP, fill = X )

def SelectLinear(): global selected; selected = 0; Interpolate()
def SelectPow(): global selected; selected = 1; Interpolate()
def SelectQuad(): global selected; selected = 2; Interpolate()
def SelectCubic(): global selected; selected = 3; Interpolate()
def SelectBack(): global selected; selected = 4; Interpolate()
def SelectCircular(): global selected; selected = 5; Interpolate()
def SelectElastic(): global selected; selected = 6; Interpolate()
def SelectBounce(): global selected; selected = 7; Interpolate()
def SelectSBounce(): global selected; selected = 8; Interpolate()
def SelectSine(): global selected; selected = 9; Interpolate()
def SelectExp(): global selected; selected = 10; Interpolate()
linButton = Button( bf, text="Linear", command=SelectLinear )
powButton = Button( bf, text="Pow", command=SelectPow )
quadButton = Button( bf, text="Quad", command=SelectQuad )
cubicButton = Button( bf, text="Cubic", command=SelectCubic )
backButton = Button( bf, text="Back", command=SelectBack )
circularButton = Button( bf, text="Circular", command=SelectCircular )
elasticButton = Button( bf, text="Elastic", command=SelectElastic )
bounceButton = Button( bf, text="Bounce", command=SelectBounce )
sBounceButton = Button( bf, text="Single Bounce", command=SelectSBounce )
sineButton = Button( bf, text="Sine", command=SelectSine )
expButton = Button( bf, text="Exponential", command=SelectExp )
linButton.pack( fill=X )
powButton.pack( fill=X )
quadButton.pack( fill=X )
cubicButton.pack( fill=X )
backButton.pack( fill=X )
circularButton.pack( fill=X )
elasticButton.pack( fill=X )
bounceButton.pack( fill=X )
sBounceButton.pack( fill=X )
sineButton.pack( fill=X )
expButton.pack( fill=X )


# Parameters

pf = Frame( topf )
pf.pack( side = LEFT )


# Acceleration

apf = Frame( pf )
apf.pack( side = TOP, fill = X  )

accLabel = Label( apf )
accLabel.config( text = "Acceleration" )
accLabel.pack( side = TOP, fill = X )

def SelectAccIn(): global accel; accel = 0; Interpolate()
def SelectAccOut(): global accel; accel = 1; Interpolate()
def SelectAccBoth(): global accel; accel = 2; Interpolate()
accInButton = Radiobutton( apf, text="In", indicatoron=0, command=SelectAccIn, value=0 )
accOutButton = Radiobutton( apf, text="Out", indicatoron=0, command=SelectAccOut, value=1 )
accBothButton = Radiobutton( apf, text="Both", indicatoron=0, command=SelectAccBoth, value=2 )
accInButton.pack( side = TOP, fill = X )
accOutButton.pack( side = TOP, fill = X )
accBothButton.pack( side = TOP, fill = X )


# Type

tpf = Frame( pf )
tpf.pack( side = TOP, fill = X  )

typeLabel = Label( tpf )
typeLabel.config( text = "Type" )
typeLabel.pack( side = TOP, fill = X )

def SelectTypeTra(): global type; type = 0;
def SelectTypeRot(): global type; type = 1;
def SelectTypeFad(): global type; type = 2;
def SelectTypeSca(): global type; type = 3;
typeTraButton = Radiobutton( tpf, text="Translation", indicatoron=0, command=SelectTypeTra, value=3 )
typeRotButton = Radiobutton( tpf, text="Rotation", indicatoron=0, command=SelectTypeRot, value=4 )
typeFadButton = Radiobutton( tpf, text="Fade", indicatoron=0, command=SelectTypeFad, value=5 )
typeScaButton = Radiobutton( tpf, text="Scale", indicatoron=0, command=SelectTypeSca, value=6 )
typeTraButton.pack( side = TOP, fill = X )
typeRotButton.pack( side = TOP, fill = X )
typeFadButton.pack( side = TOP, fill = X )
typeScaButton.pack( side = TOP, fill = X )


# Direction

dpf = Frame( pf )
dpf.pack( side = TOP, fill = X  )

accLabel = Label( dpf )
accLabel.config( text = "Direction" )
accLabel.pack( side = TOP, fill = X )

def SelectDirIn(): global direction; direction = 0;
def SelectDirOut(): global direction; direction = 1;
dirInButton = Radiobutton( dpf, text="In", indicatoron=0, width=6, command=SelectDirIn, value=7 )
dirOutButton = Radiobutton( dpf, text="Out", indicatoron=0, width=6, command=SelectDirOut, value=8 )
dirInButton.pack( side = LEFT, fill = X )
dirOutButton.pack( side = LEFT, fill = X )

# Grah type

gpf = Frame( pf )
gpf.pack( side = TOP, fill = X  )

graphLabel = Label( gpf, text="Graph" )
graphLabel.pack( side = TOP, fill = X  )

def SelectS(): global graphType; graphType = 0; Plot()
def SelectV(): global graphType; graphType = 1; Plot()
def SelectA(): global graphType; graphType = 2; Plot()
graphSButton = Button( gpf, text="S", command=SelectS )
graphVButton = Button( gpf, text="v", command=SelectV )
graphAButton = Button( gpf, text="a", command=SelectA )
graphSButton.pack( side = LEFT, fill = X )
graphVButton.pack( side = LEFT, fill = X )
graphAButton.pack( side = LEFT, fill = X )

## Modifier

#mpf = Frame( pf )
#mpf.pack( side = TOP, fill = X  )

#modLabel = Label( mpf, text="Modifier" )
#modLabel.pack( side = TOP, fill = X  )

#modEntry = Entry( mpf )
#modEntry.insert ( 0, 4 )
#modEntry.pack( side = TOP, fill = X  )

## Gravity

#gpf = Frame( pf )
#gpf.pack( side = TOP, fill = X  )

#gravLabel = Label( gpf, text="Gravity" )
#gravLabel.pack( side = TOP, fill = X  )

#gravEntry = Entry( gpf )
#gravEntry.insert ( 0, 4 )
#gravEntry.pack( side = TOP, fill = X )

# Parameter fields

midf = Frame( root )
midf.pack( side = TOP )

pmidf = Frame( midf )
pmidf.pack( side = TOP )

#timeline = Scale( pmidf, from_=0, to=7200, orient=HORIZONTAL, length=600 )
#timeline.pack( side = LEFT )

fromLabel = Label( pmidf, text="From: " )
fromLabel.pack( side = LEFT  )
fromEntry = Entry( pmidf )
fromEntry.insert ( 0, 0 )
fromEntry.pack( side = LEFT  )

toLabel = Label( pmidf, text="   To: " )
toLabel.pack( side = LEFT  )
toEntry = Entry( pmidf )
toEntry.insert ( 0, 100 )
toEntry.pack( side = LEFT  )

durLabel = Label( pmidf, text="   Duration: " )
durLabel.pack( side = LEFT  )
durEntry = Entry( pmidf )
durEntry.insert ( 0, 300 )
durEntry.pack( side = LEFT  )

# Entry fields

lmidf = Frame( midf )
lmidf.pack( side = TOP )

entryLabel = Label( lmidf, text="Type: Acceleration: Duration: Length: From: To: Modifier: Gravity: ")
entryLabel.pack( side = LEFT  )

emidf = Frame( midf )
emidf.pack( side = TOP )

#typeELabel = Label( emidf, text="Type: " )
#typeELabel.pack( side = LEFT  )
typeOptionList = ( 'Linear', 'Pow', 'Quad', 'Cubic', 'Back', 'Circular', 'Elastic', 'Bounce', 'Single Bounce', 'Sine', 'Exponential' )
typeStr = StringVar()
typeStr.set( typeOptionList[0] )
typeOm = OptionMenu( emidf, typeStr, *typeOptionList )
typeOm.pack( side = LEFT )

#accelELabel = Label( emidf, text="   Acceleration: " )
#accelELabel.pack( side = LEFT  )
binOptionList = ( '  In  ', ' Out  ' )
accelStr = StringVar()
accelStr.set( binOptionList[0] )
accelOm = OptionMenu( emidf, accelStr, *binOptionList )
accelOm.pack( side = LEFT )

#duraLabel = Label( emidf, text="   Duration: " )
#duraLabel.pack( side = LEFT  )
duraEntry = Entry( emidf, width=4 )
duraEntry.insert ( 0, 1 )
duraEntry.pack( side = LEFT  )

#lenLabel = Label( emidf, text="   Length: " )
#lenLabel.pack( side = LEFT  )
lenEntry = Entry( emidf, width=4 )
lenEntry.insert ( 0, 1 )
lenEntry.pack( side = LEFT  )

#frLabel = Label( emidf, text="   From: " )
#frLabel.pack( side = LEFT  )
frEntry = Entry( emidf, width=4 )
frEntry.insert ( 0, 0 )
frEntry.pack( side = LEFT  )

#tLabel = Label( emidf, text="   To: " )
#tLabel.pack( side = LEFT  )
tEntry = Entry( emidf, width=4 )
tEntry.insert ( 0, 1 )
tEntry.pack( side = LEFT  )

#modLabel = Label( emidf, text="   Modifier: " )
#modLabel.pack( side = LEFT  )
modEntry = Entry( emidf, width=4 )
modEntry.insert ( 0, 4 )
modEntry.pack( side = LEFT  )

#gravLabel = Label( emidf, text="   Gravity: " )
#gravLabel.pack( side = LEFT  )
gravEntry = Entry( emidf, width=4 )
gravEntry.insert ( 0, 4 )
gravEntry.pack( side = LEFT )

# Text box
    
def Compile():
    
    file = 'motion_input.txt'
    f = open( file, 'w' )
    f.write( textBox.get("1.0",END) )
    f.close()
    call(["./MotionTool", file])
    ReadFile()

botf = Frame( root )
botf.pack( side = TOP )

tf = Frame( botf )
tf.pack( side = BOTTOM, fill = X  )

textBox = Text( tf, height=15, width=118 )
textBox.pack( side = TOP )
textBox.insert( END, text )

compileButton = Button( tf, text="Compile", command=Compile )
compileButton.pack( side = TOP, fill = X )

# Set text area

def SetText( filename ):
    f = open( filename, 'r' )
    textBox.delete( 1.0, END )
    textBox.insert( END, f.read() )
    f.close()


# Translation
rad = 20
translated = w2.create_oval( 0, animH/2-rad, 2*rad, animH/2+rad, fill='blue' )

def Translate():
    
    global rad
    global translated
    w2.delete( translated )
    
    translated = w2.create_oval( 0, animH/2-rad, 2*rad, animH/2+rad, fill='blue' )
    val = ((interpolatedValues[currentValue]-min)/(max-min)) * (animW - 2*rad)
    if direction == 1: val = (animW-2*rad) - val
    w2.move( translated, val, 0 )
  
  
  
# Rotation
rot = Image.open( 'res/wheel.png' )
imgRot = ImageTk.PhotoImage( rot )
rotated = w2.create_image( animW/2, animH/2, image=imgRot )

def Rotate():

    global rot
    global imgRot
    global rotated
    w2.delete( rotated )

    val = interpolatedValues[currentValue]
    if direction == 1: val = 360 - val
    imgRot = ImageTk.PhotoImage( rot.rotate( val%360 ) )
    rotated = w2.create_image( animW/2, animH/2, image=imgRot )
    



# Fade
fade = Image.open( 'res/fade.png' )
imgFad = ImageTk.PhotoImage( fade )
faded = w2.create_image( animW/2, animH/2, image=imgFad )
    
def Fade():
    
    global fade
    global imgFad
    global faded
    w2.delete( faded )

    val = int(((interpolatedValues[currentValue]-min)/(max-min))*255)
    if direction == 1: val = 255 - val
    fade.putalpha( val )
    imgFad = ImageTk.PhotoImage( fade )
    faded = w2.create_image( animW/2, animH/2, image=imgFad )


# Scale
wheel = PhotoImage( file="res/wheel.png" )
scaled = w2.create_rectangle( animW/2, animH/2, animW/2, animH/2, fill='cyan' )

def Scale():

    global wheel
    global scaled
    w2.delete( scaled )

    val = int(((interpolatedValues[currentValue]-min)/(max-min))*100)
    if direction == 1: val = 100 - val
    wheel = PhotoImage( file="res/wheel.png" )
    wheel = wheel.subsample( val )
    #scaled = w2.create_image( animW/2, animH/2, image=wheel )
    scaled = w2.create_rectangle( animW/2-val, animH/2-val, animW/2+val, animH/2+val, fill='cyan' )

  
# Check for animation end
def AnimationCheck():
    
    global play
    global currentValue
    
    if play:
        currentValue += 1
        if currentValue >= len( interpolatedValues ) :
            currentValue = 0
            #play = False
  
  
# Animate
def Animate():
    
    w2.delete( translated )
    w2.delete( rotated )
    w2.delete( faded )
    w2.delete( scaled )
    
    if type == 0: Translate()
    elif type == 1: Rotate()
    elif type == 2: Fade()
    else: Scale()
    
    
# Draw current line
def DrawCurrentLine():
    
    global currentLine
    global currentIntersect
    
    w.delete( currentLine )
    w.delete( currentIntersect )
    
    xWidth = (endX-startX)
    yWidth = (graphMax-graphMin)
    length = len(graphValues)
    
    xVal = 0
    if length < 0.00001:
        xVal = currentValue * xWidth + startX
    else:
        xVal = float(currentValue) / length * xWidth + startX
    
    yVal = 0
    if yWidth < 0.00001:
        yVal = graphValues[currentValue] * (endY-startY) + startY
    else:
        yVal = (graphValues[currentValue]-graphMin)/yWidth * (endY-startY) + startY
    
    currentLine = w.create_line( xVal, startY, xVal, yVal, fill="black", width=2, arrow=FIRST, arrowshape=(-8,-8,5) )
    
    currentIntersect = w.create_line( startX, yVal, xVal, yVal, fill="black", width=2, arrow=FIRST, arrowshape=(-8,-8,5) )


# Loop
  
Interpolate()
  
while True:
    Animate()    
    DrawCurrentLine()
    AnimationCheck()
    root.update()
    time.sleep(0.0017)
   
root.mainloop() 
