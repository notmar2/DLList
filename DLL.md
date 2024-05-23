## Documentación de Lista Ligada Doble

### Interfaz

La interfaz comienza con las siguientes directivas:

```C++
#include <utility>
#include <iostream>
```

En este caso no tenemos guardia de header lo cual puede llegar a ser problemático, por lo que se recomienda agregar.
Esto se puede lograr con un simple `#pragma once` o de la manera tradicional con `#ifndef DLLIST.H`, `#define DLLIST.H`
y `#endif`.

Iostream es una librería que contiene funciones para controlar el flujo de información que entra y sale del programa a
través de los métodos estándar, mediante objetos como std::cin y std::cout los cuales leen y escriben a la consola.
Utility, por otro lado, es un conjunto de librerías que contienen distintas funciones y objetos de todo tipo para
realizar tareas comunes. Para este objeto se utilizarán elementos de esta librería como initializer_list, logic_error,
entre otros.

Enseguida se declara nuestro objeto, DLLList, el cual es un template con typename Object para ser utilizado con
distintos tipos de dato.

```C++
template <typename Object>
class DLList {
private:
    struct Node {
        Object data;
        Node *prev;
        Node *next;

        Node(const Object &d = Object{}, Node *p = nullptr, Node *n = nullptr);
        Node(Object &&d, Node *p = nullptr, Node *n = nullptr);
    };
```

El primer objeto público que se declara es una estructura de nombre Node, la cual se utilizará para contener el dato de
tipo Object, así como un puntero que se utilizará para mantener la dirección del siguiente nodo en la lista.

También se declaran 2 constructores. El constructor paramétrico, el cual toma un objeto del tipo de typename, y un
puntero para al nodo siguiente y se usa para copiar el valor de Object. Tiene como defaults un objeto vacío y un
nullptr; y el constructor de movimiento con un objeto, el cual cumple el mismo propósito que el constructor de copia,
pero en lugar de copiar el objeto, lo mueve directamente.

Enseguida tenemos una subclase pública llamada const_iterator.

```c++
public:
    class const_iterator{
    public:
        const_iterator();
        const Object &operator*() const;
        const_iterator &operator++();
        const_iterator operator++ (int);
        bool operator== (const const_iterator& rhs) const;
        bool operator!= (const const_iterator& rhs) const;

    protected:
        Node* current;
        Object& retrieve() const;
        const_iterator(Node *p);

        friend class DLList<Object>;
    };
```

Esta subclase se utilizará para controlar los recorridos de la lista. Pero de una forma distinta a `iterator`, puesto a
que `const_iterator` solo será usada para leer la lista, mientras que `iterator` se puede usar para modificarla.

Contiene un constructor público default, un puntero privado a un nodo y un segundo constructor de iterador, el cual es
privado y toma un puntero a un nodo como parámetro. También se declara como clase amiga (friend) lo cual le permitirá
accesar los miembros privados y protegidos de la clase DLList.

Además de esto, se declarán 5 sobrecargas de operadores públicas.

- `Object &operator*();`: Es una sobrecarga del operador de derreferenciación; se utiliza para obtener el objeto dentro
  del nodo al que apunta el iterador. Se llama de la siguiente forma `*iterador`.
- `const_iterator &operator++();`: Es una sobrecarga del operador de pre-incremento; se utiliza para incrementar el
  iterador pero retornar una referencia al iterador previa a la incrementación. Se llama de la siguiente
  forma `++iterador`.
- `const_iterator operator++(int);`: Es una sobrecarga del operador de post-incremento; se utiliza para incrementar el
  iterador y retornar el valor incrementado. Se llama de la siguiente forma `iterador++`.
- `bool operator==(const const_iterator &rhs) const;`: Es una sobrecarga del operador de igualdad; retorna verdadero si
  el objeto a comparar es igual al objeto comparado. Se llama de la siguiente forma `iterador1 == iterador2`.
- `bool operator!=(const const_iterator &rhs) const;`: Es una sobrecarga del operador de desigualdad; retorna verdadero
  si el objeto a comparar es diferente al objeto comparado. Se llama de la siguiente forma `iterador1 == iterador2`.

Los dos últimos dos operadores toman una referencia de lvalue a otro iterador como parámetro (el iterador a comparar).
El operador de post-incremento toma un entero como parámetro, este entero no sirve ningún propósito programático en el
lenguaje como tal, sin embargo, es necesario, ya que en C++ no existe una forma de diferenciar dos sobrecargas de la
misma función, método u operador basado simplemente en el tipo de dato que retorna. Por lo tanto, el compilador necesita
ese parámetro "falso" para diferenciar ambas sobrecargas.

La siguiente clase `iterator` se usa de forma similar a la anterior, pero tiene algunas diferencias, la primera, como se
mencionó, es que típicamente `iterator` se usa para leer y modificar; la otra es que esta también tiene métodos para
retroceder en la lista y para avanzar una cantidad dada.

```c++
    class iterator :
public const_iterator {
public:
iterator();
Object& operator*();
const Object& operator*() const;
iterator & operator++ ();
iterator &operator--(); // --retroceder
iterator operator++ (int);
iterator operator-- (int); // retroceder--
iterator operator+ (int steps) const; // avanzar pasos dados

protected:
iterator(Node *p);

friend class DLList<Object>;
};
```

También es importante mencionar que iterator hereda de const_iterator, lo que significa que tiene acceso a todos los miembros de const_iterator que no sean privados.

La siguiente sección declara todos los métodos públicos de nuestra clase.
Primero los constructores y el destructor:

```c++
public:
    DLList();
    DLList(std::initializer_list<Object> initList);
    ~DLList();
    DLList(const DLList &rhs);
    DLList(DLList &&rhs);
```

Se declara un constructor default, un constructor parámetrico el cual toma como parámetro un `std::initializer_list` de
tipo Object, el cual simplemente provee acceso a un arreglo de objetos de tipo Object que se utilizará para inicializar
nuestra lista; y el destructor default.
También se declara un constructor que toma un lvalue de tipo DLList, para hacer un constructor por copia y uno de rvalue
para el constructor de movimiento.

Después declaramos dos sobrecargas para los operadores de asignación por copia y movimiento:

```c++
DLList &operator=(const DLList &rhs);
DLList &operator=(DLList &&rhs);
```

Declaramos algunos métodos para los iteradores:

```c++
iterator begin();
const_iterator begin() const;
iterator end();
const_iterator end() const;
```

Se usarán para obtener el principio y el final de la lista respectivamente.

Inmediatamente, se declaran métodos para obtener el tamaño, saber si la lista está vacía y limpiarla.

```c++
int size() const;
bool empty() const;

void clear();
```

A continuación se definen `front()` y `back()`:

```c++
Object &front();
const Object &front() const;
Object &back();
const Object &back() const;
```

Retornan el objeto al frente y atrás de la lista, las sobrecargas permiten accesar y retornar objetos constantes para trabajar con listas de solo lectura.

Los siguientes métodos insertan y eliminan al frente y atrás.

```c++
void push_front(const Object &x);
void push_front(Object &&x);
void push_back(const Object &x);
void push_back(Object &&x);

void pop_front();
void pop_back();
```

Nuevamente, las sobrecargas con lvalues y rvalues son para crear versiones de copia y movimiento.

Enseguida, los métodos de inserción y borrado:

```c++
iterator insert(iterator itr, const Object &x);
iterator insert(iterator itr, Object &&x);

void insert(int pos, const Object &x);
void insert(int pos, Object &x);

iterator erase(iterator itr);
void erase(int pos);
iterator erase(iterator from, iterator to);
```

Tenemos dos tipos, el primero es por iterador y el segundo por posición.
Nuevamente, tenemos inserción de rvalue y lvalue, pero cabe mencionar que en el método por posición no hay un rvalue; esto es probablemente un error, aunque si son distintos debido a "constness".
La noción de que es un error es reforzada por el hecho de que no existe definición para esta función en el archivo CPP, pero sí para la función con `int pos, Object &&x` en los parámetros.

También es importante mencionar el método especial de borrado que toma dos iteradores para formar un rango.

Por último tenemos el método `print()` que se usa para imprimir la lista.

```c++
void print() const;
```

Las últimas líneas de código declaran los atributos privados de nuestra clase.

```c++
private:
    int theSize;
    Node *head;
    Node *tail;

    void init();
    iterator get_iterator(int pos);
};
```

Tenemos una variable de tipo entero `theSize` que guardará el tamaño de nuestra lista, dos nodos, head y tail que serán nuestro principio y final; y dos métodos, `init()` y `get_iterator()` que se usan internamente para inicializar la lista y obtener un iterador en la posición dada.

Al final del archivo se encuentra la siguiente línea:

```c++
#include "DLList.cpp"
```

Como se mencionó en la documentación de la lista sencilla, esta es una mala práctica y el código nuevamente no compila por redefinición (probado con el ambiente de CLion configurado en clase).

### Implementación

Los primeros tres elementos son la directiva de inclusión del header y los constructores de node:

```c++
#include "DLList.h"

template<typename Object>
DLList<Object>::Node::Node(const Object &d, Node *p, Node *n)
        : data{d}, prev{p}, next{n} {}

template<typename Object>
DLList<Object>::Node::Node(Object &&d, Node *p, Node *n)
        : data{std::move(d)}, prev{p}, next{n} {}
```

El primer patrón a observar es que todos los objetos inician por declararse como template y definir el scope por medio del operador de scope.
Lo único que estos constructores hacen es inicializar los elementos del nodo usando los parámetros dados con la síntaxis de inicialización.

El siguiente punto es el constructor de const_iterator:

```c++
template<typename Object>
DLList<Object>::const_iterator::const_iterator() : current{nullptr} {}
```

Como los otros, este constructor simplemente inicializa el iterador con un valor, en este caso dicho valor es un puntero nulo.

Enseguida, la sobrecarga del operador de derreferenciación, simplemente corre la función `retrieve()`. 

```c++
template<typename Object>
const Object &DLList<Object>::const_iterator::operator*() const {
    return retrieve();
}
```

El operador de pre-incrementación:

```c++
template<typename Object>
typename DLList<Object>::const_iterator &DLList<Object>::const_iterator::operator++() {
    current = current->next;
    return *this;
}
```

Como su nombre lo indica, mueve el iterador al siguiente nodo en la lista, cabe mencionar que este método se podría beneficiar de un chequeo para asegurarnos de que no estemos en el final de la lista por motivos de seguridad como en la lista sencilla, sin embargo, el código funciona sin eso.

A continuación el operador de post-incremento. Este simplemente utiliza el operador que ya habíamos declarado previamente para incrementar nuestro iterador. Antes de eso se copia el iterador anterior a un nuevo objeto y se retorna después de la incrementación.

```c++
template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::const_iterator::operator++(int) {
    const_iterator old = *this;
    ++(*this);
    return old;
}
```

Las siguientes dos sobrecargas van de la mano, la primera sobrecarga el operador `==` para regresar verdadero si los punteros actuales del objeto a comparar y el objeto comparado son iguales. El segundo regresa falso en dicho caso, pero utiliza el operador ya definido.

```c++
template<typename Object>
bool DLList<Object>::const_iterator::operator==(const const_iterator &rhs) const {
    return current == rhs.current;
}

template<typename Object>
bool DLList<Object>::const_iterator::operator!=(const const_iterator &rhs) const {
    return !(*this == rhs);
}
```

Después el método `retrieve()`:

```c++
template<typename Object>
Object &DLList<Object>::const_iterator::retrieve() const {
    return current->data;
}
```

Simplemente, retorna los datos a los que apunta el iterador actual.
Cabe mencionar que este método no está declarado en nuestro header, esto se considera mala práctica, ya que rompe con los principios bajo los cuales se recomienda separar el código en interfaz e implementación como separación de intereses, legibilidad, flexibilidad, facilidad de mantenimiento y demás.

A continuación, los constructores para nuestros iteradores:

```c++
template<typename Object>
DLList<Object>::const_iterator::const_iterator(Node *p) : current{p} {}

template<typename Object>
DLList<Object>::iterator::iterator() {}
```

Simplemente, construyen los objetos mediante los mecanismos ya mencionados.

Los operadores de derreferenciación usan la lógica que ya programamos:

```c++
template<typename Object>
Object &DLList<Object>::iterator::operator*() {
    return const_iterator::retrieve();
}

template<typename Object>
const Object &DLList<Object>::iterator::operator*() const {
    return const_iterator::operator*();
}
```

Los siguientes operadores van de la mano:

```c++
template<typename Object>
typename DLList<Object>::iterator &DLList<Object>::iterator::operator++() {
    this->current = this->current->next;
    return *this;
}

template<typename Object>
typename DLList<Object>::iterator &DLList<Object>::iterator::operator--() {
    this->current = this->current->prev;
    return *this;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator++(int) {
    iterator old = *this;
    ++(*this);
    return old;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator--(int) {
    iterator old = *this;
    --(*this);
    return old;
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::iterator::operator+(int steps) const {
    iterator new_itr = *this;
    for(int i = 0; i < steps; ++i) {
        ++new_itr;
    }
    return new_itr;
}
```

El primero, el operador de pre-incremento de iterator, convierte el nodo en current en el next del nodo actual y lo regresa, nuevamente, sería interesante tener algún chequeo para asegurarnos que no pasemos del final de la lista.

El operador de pre-decremento, hace lo mismo pero con el nodo anterior.

Los operadores de post incremento y decremento usan los operadores ya definidos para incrementar o decrementar el iterador respectivamente, pero guardan el valor previo en una variable temporal y lo envían una vez hecha la operación.

Por último, el operador de aumento por steps hace un ciclo for para usar el operador de incrementación para mover el iterador esa cantidad de pasos. Cabe mencionar, que a pesar de que se puede asumir que este método funciona tanto hacia delante como hacia atrás, ya que se pueden usar números negativos, este no es el caso porque estamos usando el ya programado método de incrementación y a que el ciclo for no corre si steps es negativo.
Es posible que esta sea la conducta deseada, en cuyo caso es recomendable añadir algún tipo de control de error para facilitar a otras personas que utilicen la estructura el darse cuenta de que la están usando mal. Si esta no es la función deseada, sin embargo, una posible solución es usar un if para dirigir el flujo cuando steps es negativo a un ciclo for que utiliza el método apropiado.

El siguiente método es un constructor para iterator:

```c++
template<typename Object>
DLList<Object>::iterator::iterator(Node *p) : const_iterator{p} {}
```

Toma un nodo como parámetro y utiliza la síntaxis de inicialización.

A continuación los constructores y el destructor de DLList:

```c++
template<typename Object>
DLList<Object>::DLList() : theSize{0}, head{new Node}, tail{new Node} {
    head->next = tail;
    tail->prev = head;
}

template<typename Object>
DLList<Object>::DLList(std::initializer_list<Object> initList) : DLList() {
    for(const auto &item : initList) {
        push_back(item);
    }
}

template<typename Object>
DLList<Object>::~DLList() {
    clear();
    delete head;
    delete tail;
}

template<typename Object>
DLList<Object>::DLList(const DLList &rhs) : DLList() {
    for(auto &item : rhs) {
        push_back(item);
    }
}

template<typename Object>
DLList<Object>::DLList(DLList &&rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail} {
    rhs.theSize = 0;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}
```

El primer constructor no toma parámetros, y simplemente crea nuevos nodos para el primer y segundo elementos. Y asigna el último elemento `tail` al segundo elemento `head->next` y viceversa asigna el elemento anterior de tail a head.

El segundo constructor toma como parámetro una lista de inicialización. Se crean los nodos como en el constructor default, y se llama al método push_back para llenar el resto de elementos de la lista.

El destructor llama el método `clear()` para limpiar la lista y después elimina head y tail para destruir el arreglo sin pérdida de memoria.

El siguiente, toma otra lista como parámetro y usa un ciclo para llamar `push_front()` en cada elemento de dicha lista, efectivamente copiándola.

El último mueve head y tail, esto debido a que head y tail ya apuntan a los demás miembros de la lista por diseño, así que no es necesario pasar cada miembro individualmente. También limpian el rhs, ya que se va a eliminar.

Los siguientes son los operadores de asignación de lista:

```c++
template<typename Object>
DLList<Object> &DLList<Object>::operator=(const DLList &rhs) {
    DLList copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template<typename Object>
DLList<Object> &DLList<Object>::operator=(DLList &&rhs) {
    std::swap(theSize, rhs.theSize);
    std::swap(head, rhs.head);
    std::swap(tail, rhs.tail);
    return *this;
}
```

El primero es la asignación por copia, asigna rhs a una variable temporal y usa swap para cambiar los valores con la lista actual. 
Cabe mencionar que esto es posiblemente un error, ya que se está llamando el operador de asignación en su propia definición, pero ya no tengo tiempo de probarlo.
Es posible que el creador buscara utilizar el constructor por copia de DLList para simplificar la programación de la siguiente forma:

```c++
template<typename Object>
DLList<Object> &DLList<Object>::operator=(const DLList &rhs) {
    DLList temp(rhs);
    std::swap(*this, temp);
    return *this;
}
```

El segundo simplemente mueve los valores de rhs ya que es un rvalue.

Ahora unas funciones misceláneas:

```c++
template<typename Object>
typename DLList<Object>::iterator DLList<Object>::begin() {
    return {head->next};
}

template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::begin() const {
    return {head->next};
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::end() {
    return {tail};
}

template<typename Object>
typename DLList<Object>::const_iterator DLList<Object>::end() const {
    return {tail};
}

template<typename Object>
int DLList<Object>::size() const {
    return theSize;
}

template<typename Object>
bool DLList<Object>::empty() const {
    return size() == 0;
}
```

Begin retorna el nodo después de next, end retorna tail, size retorna el tamaño de la lista y empty retorna `true` si el tamaño de la lista es menor a 0.

Mas métodos misceláneos:

```c++
template<typename Object>
void DLList<Object>::clear() {
    while(!empty()) {
        pop_front();
    }
}

template<typename Object>
Object &DLList<Object>::front() {
    return *begin();
}

template<typename Object>
const Object &DLList<Object>::front() const {
    return *begin();
}

template<typename Object>
Object &DLList<Object>::back() {
    return *(--end());
}

template<typename Object>
const Object &DLList<Object>::back() const {
    return *(--end());
}
```

Clear llama pop_front mientras empty no sea verdadero mientras que front y back retornan begin y el nodo antes de tail respectivamente.

Los siguientes métodos tienen en común que todos usan insert y erase para eliminar o introducir elementos en las posiciones dadas:

```c++
template<typename Object>
void DLList<Object>::push_front(const Object &x) {
    insert(begin(), x);
}

template<typename Object>
void DLList<Object>::push_front(Object &&x) {
    insert(begin(), std::move(x));
}

template<typename Object>
void DLList<Object>::push_back(const Object &x) {
    insert(end(), x);
}

template<typename Object>
void DLList<Object>::push_back(Object &&x) {
    insert(end(), std::move(x));
}

template<typename Object>
void DLList<Object>::pop_front() {
    erase(begin());
}

template<typename Object>
void DLList<Object>::pop_back() {
    erase(--end());
}

template<typename Object>
void DLList<Object>::insert(int pos, const Object &x) {
    insert(get_iterator(pos), x);
}

template<typename Object>
void DLList<Object>::insert(int pos, Object &&x) {
    insert(get_iterator(pos), std::move(x));
}

template<typename Object>
void DLList<Object>::erase(int pos) {
    erase(get_iterator(pos));
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::erase(iterator from, iterator to) {
    for(iterator itr = from; itr != to;) {
        itr = erase(itr);
    }
    return to;
}
```

El último es especial porque usa un ciclo para llamar erase en varios puntos para borrar un rango determinado.

Hablemos entonces de insert y erase:

```c++
template<typename Object>
typename DLList<Object>::iterator DLList<Object>::insert(iterator itr, const Object &x) {
    Node *p = itr.current;
    theSize++;
    return {p->prev = p->prev->next = new Node{x, p->prev, p}};
}

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::insert(iterator itr, Object &&x) {
    Node *p = itr.current;
    theSize++;
    return {p->prev = p->prev->next = new Node{std::move(x), p->prev, p}};
} 

template<typename Object>
typename DLList<Object>::iterator DLList<Object>::erase(iterator itr) {
    Node *p = itr.current;
    iterator retVal(p->next);
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    theSize--;
    return retVal;
}
```

Insert tiene dos formas, las mismas dos de siempre, por copia y por movimiento, una copia y la otra mueve los elementos.

En las listas doblemente ligadas tenemos la ventaja de que si tenemos una referencia al nodo anterior, por lo que podemos simplemente crear un nuevo nodo que toma el nuevo valor, su prev se vuelve el prev del nodo que queremos reemplazar y next se vuelve el nodo actual, después reemplazamos el next del que antes era nuestro prev por el nuevo nodo y también asignamos el nodo previo del que antes era el nodo actual y que ahora es el nodo siguiente por el nodo nuevo. 
Es interesante observar que gracias a que todos los operadores retornan el iterador se puede realizar encadenación de operadores y realizar todas estas operaciones en una sola línea.

El método de erase de igual manera ajusta nuestros punteros para que el next del nodo anterior apunte al next del actual, y el prev del siguiente apunte al prev del actual efectivamente eliminando el nodo actual de la cadena. Después se elimina el nodo actual mediante su destructor. Se retorna un iterador que apunta al nodo que antes era el siguiente y ahora es el actual.

A continuación, print y init:

```c++
template<typename Object>
void DLList<Object>::print() const {
    for(const auto &item : *this) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template<typename Object>
void DLList<Object>::init() {
    theSize = 0;
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
}
```

Print simplemente recorre la lista usando la sintaxis de for each y usa cout para imprimir cada valor individualmente.
Init como en la documentación anterior parece simplificar el proceso de inicialización, pero actualmente no está en uso.

El último método es get_iterator:

```c++
template<typename Object>
typename DLList<Object>::iterator DLList<Object>::get_iterator(int pos) {
    iterator itr = begin();
    for(int i = 0; i < pos; ++i) {
        ++itr;
    }
    return itr;
}
```

Toma una posición como parámetro y recorre la lista con iteradores hasta llegar a esa posición, retorna el iterador en ese punto.