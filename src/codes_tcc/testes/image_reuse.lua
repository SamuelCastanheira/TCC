pico.init(true)
pico.set.expert(true, 10)
pico.output.draw.image("../../../assets/imgs/dojo/agua_3.png", 
{'%', x=0.5, y=0.5, w=0.3 , h=0.3})
pico.output.draw.image("../../../assets/imgs/dojo/agua_3.png", 
{'%', x=0.7, y=0.7, w=0.3 , h=0.3})
pico.output.present(true)
pico.output.delay(3000)
pico.init(false)