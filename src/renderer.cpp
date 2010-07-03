/*
    Copyright 2007 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2007-2008 Fela Winkelmolen <fela.kde@gmail.com> 
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "renderer.h"
#include "settings.h"

#include <QPainter>

#include <QSvgRenderer>
#include <KGameTheme>
#include <kpixmapcache.h>

Renderer* Renderer::self()
{
    static Renderer instance;
    return &instance;
}

Renderer::Renderer()
{
    m_renderer = new QSvgRenderer();
    m_cache = new KPixmapCache("kbreakout");
    m_cache->setCacheLimit(3*1024); // TODO: put somewhere else!!!!!!!! (also in KNetWalk!!)

    if(!loadTheme(Settings::theme()))
        kDebug() << "Failed to load any game theme!";
}

Renderer::~Renderer()
{
    delete m_renderer;
    delete m_cache;
}

bool Renderer::loadTheme(const QString& themeName)
{
    // variable saying whether to discard old cache upon 
    // successful new theme loading
    // we won't discard it if m_currentTheme is empty meaning that
    // this is the first time loadTheme() is called
    // (i.e. during startup) as we want to pick the cache from disc
    bool discardCache = !m_currentTheme.isEmpty();

    if(!m_currentTheme.isEmpty() && m_currentTheme == themeName) {
        kDebug() << "Notice: not loading the same theme";
        return true; // this is not an error
    }
    KGameTheme theme;
    if (!theme.load(themeName))  {
        kDebug() << "Failed to load theme" << Settings::theme();
        kDebug() << "Trying to load default";
        if(!theme.loadDefault())
            return false;
    }

    m_currentTheme = themeName;

    bool res = m_renderer->load( theme.graphics() );
    kDebug() << "loading" << theme.graphics();
    if (!res) return false;

    if (discardCache) {
        kDebug() << "discarding cache";
        m_cache->discard();
    }
    return true;
}

QPixmap Renderer::renderedSvgElement(const QString &id, const QSize &size) const
{
    int const w = size.width();
    int const h = size.height();

    QPixmap pixmap;
    QString cacheName = 
            QString("%1-%2x%3").arg(id).arg(w).arg(h);
    if(m_cache->find(cacheName, pixmap)) return pixmap;
    
    // else
    kDebug() << "re-rendering pixmap" << cacheName;
    pixmap = QPixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    
    if (!m_renderer->elementExists(id)) {
        kDebug() << "Invalid elementId: " << id;
        return pixmap;
    }
    
    m_renderer->render(&p, id/*, size*/);
    p.end();
    m_cache->insert(cacheName, pixmap);
    kDebug() << "cache size:" << m_cache->size() << "kb";
    
    return pixmap;
}
