import string
import random
from numpy.random import randint
from datetime import datetime


keywords = ['init # # #','val #', 'existeix #', 'quants', 'elimina #', 'assig # #', '# destroy']

def buildTest(templateStr, params):
	i = 0
	while "#" in templateStr:
		templateStr = templateStr.replace('#', str(params[i]), 1)
		i += 1
	return templateStr + '\n'

def crearCabecera(id, nomCataleg, numClaus):
	return '##!------------------------------------------------------------------\n## ID: {}\n## Nom: {}\n## {} claus\n##!------------------------------------------------------------------\n'.format(id, nomCataleg, numClaus)

fileName = 'cataleg-' + datetime.now().strftime('%d-%m-%Y-%H-%M-%S') + '.inp'
fileSave = open(fileName, 'x')

maxElems = 50
for i in range(1, maxElems+1):

	fileSave.write('start_t\n')

	objName = ''.join(random.choices(string.ascii_uppercase + string.digits, k=12))
	numDicc = randint(50000, 150000)
	# Cabecera del catalogo
	fileSave.write(crearCabecera(i, objName, numDicc))
	claves = []

	# Creacion del catalogo
	fileSave.write(buildTest(keywords[0], [objName, 'cataleg<int>', numDicc]))

	# Assignar X entradas al diccionario
	for _ in range(0, numDicc):
		claves.append(''.join(random.choices(string.ascii_uppercase + string.digits, k=5)))
		fileSave.write(buildTest(keywords[5], [claves[-1:][0], randint(0, 500)]))
		if randint(0, 100) % 2 == 0:
			fileSave.write(buildTest(keywords[4], [claves[-1:][0]]))
			# claves.pop(-1)

	# Mirar si existen
	random.shuffle(claves)
	for check in claves:
		if randint(0, 100) % 2 == 0:
			fileSave.write(buildTest(keywords[2], [check]))

	# Quants?
	fileSave.write(buildTest(keywords[3], []))
	fileSave.write('##! Habia originalmente {} claves\n'.format(len(claves)))


	# Ver el valor de todas las claves aun que estén eliminadas
	random.shuffle(claves)
	for check in claves:
		fileSave.write(buildTest(keywords[1], [check]))

	fileSave.write(buildTest(keywords[6], [objName]))

	fileSave.write('stop_t\n')



fileSave.close()


