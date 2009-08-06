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

#ifndef RENDERER_H
#define RENDERER_H

#include <QPixmap>

class KSvgRenderer;
class KPixmapCache;
/**
 * This class is responsible for rendering all the game graphics.
 * Graphics is rendered from svg file specified by current theme.
 * Only one instance of this class exists during a program run.
 * It can be accessed with static function Renderer::self().
 */
class Renderer
{
public:
    static Renderer* self();
    bool loadTheme(const QString& themeName);
    QPixmap renderedSvgElement(const QString &id, const QSize &size) const;
    
private:
    // disable copy - it's singleton
    Renderer();
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
    ~Renderer();

    KSvgRenderer *m_renderer;
    KPixmapCache *m_cache;
    QString m_currentTheme;
};

#endif
