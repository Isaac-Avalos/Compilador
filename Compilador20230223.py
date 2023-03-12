ERR = -1
ACP = 99
idx = 0
cERR = False
tok = ''
lex = ''
bPrinc = False
ren = 1
colu = 0
def erra(terr, desc):
    global ren, colu
    global cERR
    print('['+str(ren)+']'+'['+str(colu)+']', terr, desc)
    cERR = True

matran=[
    #let  dig  del  opa   <    >    =    .   "
    [1,   2,   6,   5,    10,  8,   7,  ERR, 12], #0
    [1,   1,   ACP, ACP, ACP, ACP, ACP, ACP,ACP], #1
    [ACP, 2,   ACP, ACP, ACP, ACP, ACP,  3, ACP], #2
    [ERR, 4,   ERR, ERR, ERR, ERR, ERR, ERR,ERR], #3
    [ACP, 4,   ACP, ACP, ACP, ACP, ACP, ACP,ACP], #4
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP], #5
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP], #6
    [ACP, ACP, ACP, ACP, ACP, ACP,  9,  ACP,ACP], #7
    [ACP, ACP, ACP, ACP, ACP, ACP,  9,  ACP,ACP], #8
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP], #9
    [ACP, ACP, ACP, ACP, ACP, 11,    9, ACP,ACP], #10
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP], #11
    [12,   12,  12,  12,  12,  12,  12,  12, 13], #12
    [ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,ACP]  #13
]

tipo = ['nulo', 'entero', 'decimal', 'palabra', 'logico']
opl = ['no', 'y', 'o']
ctl= ['verdadero', 'falso']
key= ['constante', 'desde', 'si', 'hasta', 'mientras', 'entero', 'decimal', 'regresa', 'hacer',
      'palabra', 'logico', 'nulo', 'sino', 'incr' 'imprime', 'imprimenl', 'lee', 'repite', 'que']
opar=['+', '-', '*', '/', '%', '^']
deli=[';', ',', '(',')', '{', '}', '[', ']', ':']
delu=[' ', '\t', '\n']
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

def opno(): pass

def opy():
    opr = 'y'
    while opr == 'y':
        opno()
        opr = lex

def expr():
    opr = 'o'
    while opr == 'o':
        opy()
        opr = lex

def constVars():
        global entrada, idx, tok, lex
        tok, lex = scanner()

def params(): 
    global entrada, lex, tok
    tok, lex = scanner()

def leer(): pass

def imprime(): pass

def imprimenl(): pass

def desde(): pass

def mientras(): pass

def si(): pass

def repite(): pass

def lmp(): pass

def regresa(): pass

def comando(): 
    global tok, lex
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
    tok, lex = scanner();
    while lex != ';':
        if lex != ';': comando()
        tok, lex = scanner();
        if lex != ';': erra('Error de Sintaxis', 'se esperaba ; y llego '+lex)
        tok, lex = scanner()

def blkFunc():
    global lex, tok
    if lex != '{': erra('Error de Sintaxis', 'se esperaba abrir \"{\" y llego '+lex)
    tok, lex = scanner()
    if lex != '}': estatutos()
    if lex != '}':erra('Error de Sintaxis', 'se esperaba cerrar \"}\" y llego '+lex)



def funcs():
        global entrada, idx, tok, lex, tipo, bPrinc
        print('Entro a Funcs')
        if not(lex in tipo):
            erra('Error Sintactico', 'Se esperaba tipo' + str(tipo))
        tok, lex = scanner()
        if tok != 'Ide': erra('Error Sintaxis', 'Se esperaba Nombre Funcion y llego ' + lex)
        if bPrinc: erra('Error de Semantica', 'la Funcion Principal ya esta definida') 
        if lex == 'principal': bPrinc = True
        tok, lex = scanner()      
        if lex != '(': erra('Error de Sintaxis', 'se esperaba parentisis abierto \"(\" y llego '+ lex) 
        tok, lex = scanner()      
        if lex != ')': params()
        if lex != ')': erra('Error de Sintaxis', 'se esperaba parentisis cerrado \")\"')
        tok, lex = scanner()
        blkFunc()

def prgm():
    while len(entrada) > 0 and  idx < len(entrada):
        constVars()
        funcs();

def parser():
    global entrada, idx, tok, lex
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
    if not(cERR): print('Programa COMPILO con EXITO') 