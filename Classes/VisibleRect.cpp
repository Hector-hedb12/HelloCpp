/*
 * Clase auxiliar obtenida de Cocos2d-x, utilizada para el manejo del rectangulo
 * visible de la pantalla. Esta clase es utilizada para la colocacion de los
 * elementos de la interfaz
 */

#include "VisibleRect.h"

CCRect VisibleRect::s_visibleRect;

// Obtiene el origen y el tamaño del rectangulo de la pantalla del usuario
void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        s_visibleRect.origin = pEGLView->getVisibleOrigin();
        s_visibleRect.size = pEGLView->getVisibleSize();
    }
}

// Funcion que retorna el rectangulo visible
CCRect VisibleRect::getVisibleRect()
{
    lazyInit();
    return CCRectMake(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

// Funcion que retorna las coordenadas del punto izquierdo central de la pantalla
CCPoint VisibleRect::left()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

// Funcion que retorna las coordenadas del punto derecho central de la pantalla
CCPoint VisibleRect::right()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

// Funcion que retorna las coordenadas del punto de arriba central de la pantalla
CCPoint VisibleRect::top()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

// Funcion que retorna las coordenadas del punto de abajo central de la pantalla
CCPoint VisibleRect::bottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

// Funcion que retorna las coordenadas del centro de la pantalla
CCPoint VisibleRect::center()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

// Funcion que retorna las coordenadas de la esquina superior izquierda de la pantalla
CCPoint VisibleRect::leftTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

// Funcion que retorna las coordenadas de la esquina superior derecha de la pantalla
CCPoint VisibleRect::rightTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

// Funcion que retorna las coordenadas de la esquina inferior izquierda de la pantalla
CCPoint VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

// Funcion que retorna las coordenadas de la esquina inferior derecha de la pantalla
CCPoint VisibleRect::rightBottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}
