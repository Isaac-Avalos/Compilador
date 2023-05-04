import os
ERR = -1
ACP = 99
idx = 0
cERR = False
tok = ''
lex = ''
bPrinc = False
ren = 1
colu = 0
pTipos = []

cTipo = ["E=E", "P=P", "D=D", "L=L", "D=E",
        "E+E", "E+D", "D+E", "D+D", "P=P",
        "E-E", "E-D", "D-E", "D-D",
        "E*E", "E*D", "D*E", "D*D",
        "E/E", "E/D", "D/E", "D/D",
        "E%E", "-E", "-D",
        "LyL", "LoL", "noL",
        "E>E", "D>E", "E>D", "D>D",
        "E<E", "D<E", "E<D", "D<D",
        "E>=E", "D>=E", "E>=D", "D>=D",
        "E<=E", "D<=E", "E<=D", "D<=D",
        "E<>E", "D<>E", "E<>D", "D<>D", "P<>P",
        "E==E", "D==E", "E==D", "D==D", "P==P"
]

tipoR = ["",  "",  "",  "",  "",
        "E", "D", "D", "D", "P",
        "E", "D", "D", "D",
        "E", "D", "D", "D",
        "D", "D", "D", "D",
        "E", "E", "D",
        "L", "L", "L",
        "L", "L", "L", "L",
        "L", "L", "L", "L",
        "L", "L", "L", "L",
        "L", "L", "L", "L",
        "L", "L", "L", "L", "L",
        "L", "L", "L", "L", "L"
]

def buscaTipo(cadt):
    for i in range(len(cTipo)):
        if cTipo[i]==cadt: return i
    return -1


class objPrgm():
    def __init__(self, nom, cls, tip, dim1, dim2, apv) -> None:
        self.dim2apv = []
        self.apv = ["0" for i in range(1000)]
        self.nombre = nom
        self.clase = cls
        self.tipo = tip
        self.dim1 = dim1
        self.dim2 = dim2
        self.apv[int(dim1)] = apv
        return

class TabSimb():
    arreglo = []
    def inserSimbolo(self, nom, cls, tip, dim1, dim2, apv):
        ox = self.buscaSimbolo(nom)
        if ox != None:
            if ox.clase == 'V': cls = 'Variable'
            elif ox.clase == 'C': cls = 'Constante'
            elif ox.clase == 'F': cls = 'Funcion'
            elif ox.clase == 'I': cls = 'Indefinido'
            elif ox.clase == 'L': cls = 'Variable Local'
            if ox.tipo == 'E': tp = 'Entero'
            elif ox.tipo == 'D': tp = 'Decimal'
            elif ox.tipo == 'P': tp = 'Palabra'
            elif ox.tipo == 'I': tp = 'Nulo o Indefinido'
            elif ox.tipo == 'L': tp = 'Logico'
            erra('Error Semantico ' + nom + ' ya esta declarado como', cls + ' del tipo ' + tp)
            return
        obj = objPrgm(nom, cls, tip, dim1, dim2, apv)
        self.arreglo.append( obj )
        return

    def buscaSimbolo(self, ide):
        for x in self.arreglo:
            if x.nombre == ide: return x
        return None
    
    def grabaTabla(self, archSal):
        with open(archSal, 'w') as aSal:
            for x in self.arreglo:
                aSal.write(x.nombre +',' + x.clase + ',' + x.tipo + ',' + \
                           x.dim1 + ','+ x.dim2 + ',' + x.apv[0] + ',#\n')
            aSal.write('@\n')
            aSal.close()
        return
    
    def impTabSim(self):
        for x in self.arreglo:
            print(x.nombre + ',' + x.clase + ',' + x.tipo + ',' + x.dim1 + ',' + x.dim2 + ',' + x.apv[0]+ ',#')
        print('@')
        return

class codigo():
    def __init__(self, mnem, dir1, dir2):
        self.mnemo = mnem
        self.dir1 = dir1
        self.dir2 = dir2

linCod = 0

class Programa():
    cod = []
    def __init__(self):
        for i in range(10000):
            x = codigo('LIT', '0', '0')
            self.cod.append(x)
        return

    def insCodigo(self, mnemo, dir1, dir2):
        global linCod
        linCod = linCod + 1
        x = codigo(mnemo, dir1, dir2)
        self.cod[linCod] = x
        return
    
    def impCodigo(self):
        global linCod
        for i in range(linCod + 1):
           if i > 0:
              x = self.cod[i]
              print(str(i)+ ' '+x.mnemo + ' ' + x.dir1 + ', ' + x.dir2)
        return

    def grabaCodigo(self, archSal):
        global linCod
        with open(archSal, 'r') as aSal:
            if aSal == None: return 

        if linCod > 0:
            with open(archSal, 'a') as aSal:
                for i in range(linCod + 1):
                    if i > 0:
                        x = self.cod[i]
                        aSal.write(str(i)+ ' ' + x.mnemo + ' ' + x.dir1 + ',' + x.dir2 + '\n')

            aSal.close()
        return



tabSimb = TabSimb()  

prgmCod = Programa()

pilaTipos = []

def erra(terr, desc):
    global ren, colu
    global cERR
    print('['+str(ren)+']'+'['+str(colu)+']', terr, desc)
    cERR = True

matran=[
    #let  dig  del  opa   <    >    =    .    "
    [1,   2,   6,   5,    10,  8,   7,  ERR,  12], #0
    [1,   1,   ACP, ACP, ACP, ACP, ACP, ACP, ACP], #1
    [ACP, 2,   ACP, ACP, ACP, ACP, ACP,  3,  ACP], #2
    [ERR, 4,   ERR, ERR, ERR, ERR, ERR, ERR, ERR], #3
    [ACP, 4,   ACP, ACP, ACP, ACP, ACP, ACP, ACP], #4
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP], #5
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP], #6
    [ACP, ACP, ACP, ACP, ACP, ACP,  9,  ACP, ACP], #7
    [ACP, ACP, ACP, ACP, ACP, ACP,  9,  ACP, ACP], #8
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP], #9
    [ACP, ACP, ACP, ACP, ACP, 11,    9, ACP, ACP], #10
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP], #11
    [12,   12,  12,  12,  12,  12,  12,  12,  13], #12
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP]  #13
]

tipo = ['nulo', 'entero', 'decimal', 'palabra', 'logico']
opl = ['no', 'y', 'o']
ctl= ['verdadero', 'falso']
key= ['constante', 'desde', 'si', 'hasta', 'mientras', 'entero', 'decimal', 'regresa', 'hacer',
      'palabra', 'logico', 'nulo', 'sino', 'incr' 'imprime', 'imprimenl', 'lee', 'repite', 'que']
opar=['+', '-', '*', '/', '%', '^']
deli=[';', ',', '(',')', '{', '}', '[', ']', ':']
delu=[' ', '\t', '\n']
opRl = ['<', '>', '<=', '>=', '<>']
tkCts = ['Ent', 'Dec', 'CtA', 'CtL']
entrada = ''
def colCar(x):
    if x == '_' or x.isalpha(): return 0 
    if x.isdigit(): return 1
    if x in deli: return 2
    if x in opar: return 3
    if x == '<': return 4   
    if x == '>': return 5   
    if x == '=': return 6   
    if x == '.': return 7
    if x == '"': return 8
    if x in delu: return 15
    erra('Error Lexico', x + ' simbolo no valido en Alfabeto')
    return ERR


def scanner():
    global entrada, ERR, ACP, idx, ren, colu
    estado = 0
    lexema = ''
    c = ''
    col = 0
    while idx < len(entrada) and \
          estado != ERR and estado != ACP: 
          c = entrada[idx]
          idx = idx + 1
          if c == '\n':
              colu = 0
              ren = ren + 1

          col = colCar(c)
          if estado == 0 and col == 15: 
            continue;
          if col >= 0 and col <= 8 or col == 15:
            if col == 15 and estado != 12: 
                estado = ACP
            if col >=0 and col <= 8:
                estado = matran[estado][col]
            if estado != ERR and estado != ACP and col != 15 or col == 15 and estado == 12:
                estA = estado
                lexema = lexema + c
            
            if c != '\n': colu = colu + 1

    if estado != ACP and estado != ERR: estA = estado;
    token = 'Ntk'
    if estado == ACP and col != 15: 
        idx = idx - 1
        colu = colu - 1

    if estado != ERR and estado != ACP:
        estA = estado
    
    if lexema in key: token = 'Res'
    elif lexema in opl: token = 'OpL'
    elif lexema in ctl: token = 'CtL'
    else: token = 'Ide'

    if estA == 2: token = 'Ent'
    elif estA == 4: token = 'Dec'
    elif estA == 5: token = 'OpA'
    elif estA == 6: token = 'Del'
    elif estA == 7: token = 'OpS'
    elif estA in [8, 9 , 10 , 11]: token = 'OpR'
    elif estA == 13: token = 'CtA'

    if token == 'Ntk':
        print('estA=', estA, 'estado=', estado)

    return token, lexema

def cte():
    global tok, lex, prgmCod, pilaTipos, tabSimb
    if not(tok in tkCts):
        erra('Error de sintaxis', 'se esperaba Cte y llego '+ lex) 
    else:
        if tok == 'CtA' or tok == 'Ent' or tok == 'Dec':
           if tok == 'Dec': pilaTipos.append('D')
           if tok == 'Ent': pilaTipos.append('E')
           if tok == 'CtA': 
               lex = lex[1:len(lex)-1]
               pilaTipos.append('P')
           prgmCod.insCodigo('LIT', lex, '0')
        if tok == 'CtL':
            pilaTipos.append('L')
            if lex == 'verdadero':
                prgmCod.insCodigo('LIT', 'V', '0')
            elif lex == 'falso':
                prgmCod.insCodigo('LIT', 'F', '0')

def termino():
    global lex, tok, tabSimb, pilaTipos
    if lex != '(' and tok != 'Ide' and tok != 'CtA' and \
        tok != 'CtL' and tok != 'Ent' and tok != 'Dec':
        tok, lex = scanner()
    if lex == '(':
        tok, lex = scanner()
        expr()
        if lex != ')':
            erra('Error de Sintaxis', 'se espera cerrar ) y llego '+ lex)
    elif tok == 'Ide':
        nomIde = lex

        x = tabSimb.buscaSimbolo()
        if x != None:
            pilaTipos.append(x.tipo)
        else:
            pilaTipos.append('I')
            erra('Error Semantico', 'Identificador no declarado ' + nomIde)

        tok, lex = scanner()
        if lex == '[': 
            tok, lex = scanner()
            expr()
            if lex != ']':
                erra('Error Sintaxis', 'se esperaba cerrar ] y llego '+lex)
        elif lex == '(': asigLfunc()
        prgmCod.insCodigo('LOD', nomIde, '0')
        oIde = tabSimb.buscaSimbolo(nomIde)
        if oIde != None:
            pilaTipos.append(oIde.tipo)
        else:
            erra("Error de Semantica", 'Identificador no declarado '+ nomIde) 
            pilaTipos('I')
    elif tok == 'CtL' or tok == 'CtA' or tok == 'Dec' or tok == 'Ent': 
        cte()
    if lex != ')' and lex != ',': 
        tok, lex = scanner()

def signo():
    global lex, tok
    if lex == '-':
        tok, lex = scanner()
    termino()


def expo():
    global tok, lex
    opr = '^'
    while opr == '^':
        signo()
        opr = lex


def multi():
    global tok, lex
    opr = '*'
    while opr == '*' or opr == '/' or opr == '%':
        expo()
        opr = lex

def suma():
    global tok, lex
    opr = '+'
    while opr == '+' or opr == '-':
        multi()
        opr = lex

def oprel():
    global tok, lex
    opr = '<'
    while opr in opRl:
        suma()
        opr = lex

def opno(): 
    global lex, tok
    if lex == 'no':
        tok, lex = scanner()
    oprel()

def opy():
    global tok, lex
    opr = 'y'
    while opr == 'y':
        opno()
        opr = lex

def expr():
    global tok, lex
    opr = 'o'
    while opr == 'o':
        opy()
        opr = lex

def dimen(): pass

def constants():
    global tok, lex
    tok, lex = scanner()
    if lex in tipo:
            idTipo = lex
            tok, lex = scanner()
    while tok == 'Ide':
        Iden = lex
        tok, lex = scanner()
        if lex == '[':
            tok, lex = scanner()
            dimen()
        elif lex == '=':
            tok, lex = scanner()
            if idTipo == 'entero': idTipo = 'E'
            elif idTipo == 'decimal': idTipo = 'D'
            elif idTipo == 'logico': idTipo = 'L'
            elif idTipo == 'palabra': idTipo = 'P'
            if idTipo == 'P': lex = lex[1:len(lex)-1]
            tabSimb.inserSimbolo(Iden, 'C', idTipo, '0', '0', lex)
        
        tok, lex = scanner()
        if lex == ',':
            tok, lex = scanner()

    if lex != ';':
         erra('Error de Sintaxis', 'se esperaba <;> y llego '+ lex)

    return

def constVars():
        global idx, tok, lex, bPrinc
        if lex == 'constante': constants()
        else:
            if lex in tipo:
                idTipo = lex
                if idTipo == 'entero': idTipo = 'E'
                elif idTipo == 'decimal': idTipo = 'D'
                elif idTipo  == 'logico': idTipo = 'L'
                elif idTipo == 'palabra': idTipo = 'P'
            else: erra('se esperaba tipo '+ str(tipo), ' y llego '+lex)
            tok, lex = scanner()
            nIde = lex
            if tok != 'Ide': erra('Se esperaba Identificador', 'y llego '+ lex)
            tok, lex = scanner()
            if lex == ',':
                tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')
                tok, lex = scanner()
                while tok == 'Ide':
                    nIde = lex
                    tok, lex = scanner()
                    if lex != ',' and lex  != '=' and lex != ';': erra('Error de Sintaxis se esperaba <, = ;>', 'y llego '+ lex)
                    elif lex == ',':
                        tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')
                        tok, lex = scanner()
                    elif lex == '=':
                        tok, lex = scanner()
                        if tok != 'CtA' and idTipo == 'P': erra("Error Semantico, se esperaba Cte palabra ", "y llego "+ lex)
                        elif tok != 'CtL' and idTipo == 'L': erra("Error Semantico, se esperaba Cte logico ", "y llego "+ lex)
                        elif tok != 'Ent' and idTipo == 'E': erra("Error Semantico, se esperaba Cte entero ", "y llego "+ lex)
                        elif tok != 'Dec' and idTipo == 'D': erra("Error Semantico, se esperaba Cte decimal ", "y llego "+ lex)
                        else:
                            if tok == 'CtA': lex = lex[1:len(lex)-1] 
                            tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', lex)
                        tok, lex = scanner()
                        if lex == ',': tok, lex = scanner()
                if lex == ';': tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')        
            elif lex == '=':
                tok, lex = scanner();
                if tok != 'CtA' and idTipo == 'P': erra("Error Semantico, se esperaba Cte palabra ", "y llego "+ lex)
                elif tok != 'CtL' and idTipo == 'L': erra("Error Semantico, se esperaba Cte logico ", "y llego "+ lex)
                elif tok != 'Ent' and idTipo == 'E': erra("Error Semantico, se esperaba Cte entero ", "y llego "+ lex)
                elif tok != 'Dec' and idTipo == 'D': erra("Error Semantico, se esperaba Cte decimal ", "y llego "+ lex)
                else:
                    if tok == 'CtA': lex = lex[1:len(lex)-1] 
                    tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', lex)
                tok, lex = scanner()
                if lex != ',' and lex != ';': erra('Error de Sintaxis, se esperaba , o ;', 'y llego '+ lex)
                elif lex == ',':
                    tok, lex = scanner()
                    while tok == 'Ide':
                        nIde = lex
                        tok, lex = scanner()
                        if lex != ',' and lex  != '=' and lex != ';': erra('Error de Sintaxis se esperaba , =, ;', 'y llego '+ lex)
                        elif lex == ',':
                            tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')
                            tok, lex = scanner()
                        elif lex == '=':
                            tok, lex = scanner()
                            if tok != 'CtA' and idTipo == 'P': erra("Error Semantico, se esperaba Cte palabra ", "y llego "+ lex)
                            elif tok != 'CtL' and idTipo == 'L': erra("Error Semantico, se esperaba Cte logico ", "y llego "+ lex)
                            elif tok != 'Ent' and idTipo == 'E': erra("Error Semantico, se esperaba Cte entero ", "y llego "+ lex)
                            elif tok != 'Dec' and idTipo == 'D': erra("Error Semantico, se esperaba Cte decimal ", "y llego "+ lex)
                            else:
                                if tok == 'CtA': lex = lex[1:len(lex)-1] 
                                tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', lex)
                                tok, lex = scanner()
                                if lex != ',' and lex != ';': erra("Error de Sintaxis se esperaba ,", 'y llego '+lex)
                                if lex == ',':tok, lex = scanner()
                    if lex != ';': erra('Error de Sintaxis, se esperaba ; ', 'y llego '+lex)  
                    else: tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')   
            elif lex == '(': 
                if bPrinc and nIde == 'principal': erra('Error de Semantica', 'la Funcion Principal ya esta definida') 
                if nIde == 'principal': 
                    bPrinc = True
                    tabSimb.inserSimbolo('_P', 'I', 'I', str(linCod + 1), '0', '0')
                tok, lex = scanner()      
                funcs()
                if nIde == 'principal': 
                    prgmCod.insCodigo('OPR', '0', '0')
                    idTipo = 'I'
            else:
                if lex == ';':
                    tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')
                while lex != ';':
                    if lex == '[': dimen()
                    elif lex == ',':
                        tabSimb.inserSimbolo(nIde, 'V', idTipo, '0', '0', '0')
                        tok, lex = scanner()
                        if tok == 'Ide':
                            nIde = lex
                        if tok != 'Ide': erra('Se esperaba Identificador', 'y llego '+ lex)
                        tok, lex = scanner()
                tok, lex = scanner()
            idClase = 'V'
            if nIde == 'principal': idClase = 'F'
            if tabSimb.buscaSimbolo(nIde) == None:
                tabSimb.inserSimbolo(nIde, idClase, idTipo, '0', '0', '0')
        return



def params(): 
    global entrada, lex, tok
    tok, lex = scanner()
    


def gpoExp():
    global tok, lex, prgmCod
    deli=','
    while deli == ',':
        expr()
        if lex == ',': 
            deli = lex
            prgmCod.insCodigo('OPR', '0', '20')
            tok, lex = scanner()
        elif lex == ')': break


def leer(): pass

def imprime(): 
    global tok, lex, prgmCod
    tok, lex = scanner()
    if lex != '(':
        erra('Error de Sintaxis', 'se esperaba abrir ( y llego '+ lex)
    tok, lex = scanner()
    if lex == ')': prgmCod.insCodigo('LIT', '', '0')
    elif lex != ')': 
        gpoExp()
    if lex != ')':
        erra('Error de Sintaxis', 'se esperaba cerrar ) y llego '+ lex)
    prgmCod.insCodigo('OPR', '0', '20')

def imprimenl(): 
    global tok, lex, prgmCod
    tok, lex = scanner()
    if lex != '(':
        erra('Error de Sintaxis', 'se esperaba abrir ( y llego '+ lex)
    tok, lex = scanner()
    if lex == ')': prgmCod.insCodigo('LIT', '', '0')
    if lex != ')': 
        gpoExp()
    if lex != ')':
        erra('Error de Sintaxis', 'se esperaba cerrar ) y llego '+ lex)
    prgmCod.insCodigo('OPR', '0', '21')


def desde(): pass

def mientras():'''
    global tok, lex
    tok, lex = scanner()

    if lex == 'que':
        expr()
    if lex == '{':
        blkcmd()
    if lex != '':
        
        '''


def si(): pass

def repite(): pass

def lmp():
    prgmCod.insCodigo('OPR', '0', '18')

def regresa(): pass

def Lfunc(): pass

def udim(): pass

def asigna(): 
    global tok, lex, tabSimb, pilaTipos
    nomIde = lex

    tabSimb.impTabSim()
    x = tabSimb.buscaSimbolo(nomIde)
    if x != None:
        pilaTipos.append(x.tipo)
    else:
        erra('Error de semantica', 'Identificador no declarado' + nomIde)
        pilaTipos.append('I')
    tok, lex = scanner()
    if lex == '[': udim()
    if lex == '=': 
        pilaTipos.append('=')
    else:
        erra('Error de sintaxis', 'se esperaba = y llego' + lex)
    tok, lex = scanner()
    expr()
    # Valida tipo en asignacion
    tipd = pilaTipos.pop()
    tips = pilaTipos.pop() + tipd
    tipi = pilaTipos.pop()
    tips = tipi + tips
    i = buscaTipo(tips)
    if i >=0 and tipoR[i] != '':
        pilaTipos.append(tipoR[i])
    elif i < 0:
        erra('Error semantico', ' conflicto en tipos en asignacion de ' + tipi + ' = ' + tipd)







def comando(): 
    global tok, lex
    if tok == 'Ide': asigna()
    if lex == 'lee': leer()
    elif lex == 'imprime': imprime()
    elif lex == 'imprimenl': imprimenl()
    elif lex == 'desde': desde()
    elif lex == 'mientras': mientras()
    elif lex == 'si': si()
    elif lex == 'repite': repite()
    elif lex == 'lmp': lmp()
    elif lex == 'regresa': regresa()
    else: erra('Error de Sintaxis', 'comando no definido '+ lex)
    tok, lex = scanner()

def blkcmd():
    global lex, tok
    tok, lex = scanner()
    if lex != ';' and lex != '{': 
        comando()
        tok, lex = scanner()
        if lex != ';': erra('Error de Sintaxis', 'se esperaba ; y llego '+lex)
    elif lex == '{':
        estatutos()
        if lex != '}': erra('Error de Sintaxis', 'se esperaba cerrar block \"}\" y llego '+ lex)

def estatutos(): 
    global tok, lex
    cbk = '{'
    while cbk != '}':
        if lex != ';': comando()
        if lex != ';': erra('Error de Sintaxis', 'se esperaba ; y llego '+lex)
        tok, lex = scanner()
        cbk = lex

def blkFunc():
    global lex, tok
    if lex != '{': erra('Error de Sintaxis', 'se esperaba abrir \"{\" y llego '+lex)
    tok, lex = scanner()
    if lex != '}': estatutos()
    if lex != '}':erra('Error de Sintaxis', 'se esperaba cerrar \"}\" y llego '+lex)



def funcs():
        global tok, lex, tipo, bPrinc
        if lex != ')': params()
        if lex != ')': erra('Error de Sintaxis', 'se esperaba parentisis cerrado \")\"')
        tok, lex = scanner()
        blkFunc()


def prgm():
    global entrada, idx, tok, lex
    while len(entrada) > 0 and  idx < len(entrada):
        tok, lex = scanner()
        constVars()

def parser():
    prgm()

if __name__ == '__main__':
    arche = input('Archivo (.icc) [.]=salir: ')
    if arche == '.': exit()
    archivo = open(arche, 'r')
    #se carag archivo en entrada
    entrada = ''
    for linea in archivo:
        entrada += linea
        
    print(entrada)
    parser()
    archs = arche[:len(arche)-4]
    archsl = archs + '.eje'
    
    if not(cERR): 
        print('Programa COMPILO con EXITO') 
        tabSimb.grabaTabla(archsl)
        prgmCod.grabaCodigo(archsl)
        os.system('Inter ' + archs)