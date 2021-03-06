/**
 *  OSM
 *  Copyright (C) 2018  Pavel Smokotnin

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WINDOWFUNCTION_H
#define WINDOWFUNCTION_H

#include <QDebug>
#include <QVariant>
#include <math.h>
#include "container/array.h"

class WindowFunction : QObject
{
    Q_OBJECT

public:
    enum Type {rectangular, hann, hamming, flat_top, blackman_harris, HFT223D};
    Q_ENUM(Type)
    static const std::map<Type, QString> TypeMap;

private:
    Type m_type;
    unsigned int m_size;
    container::array<float> m_data;
    float m_gain;

    //calculate data for current type
    void calculate();

public:
    explicit WindowFunction(Type type, QObject *parent = nullptr);

    void setSize(unsigned int size);
    unsigned int size() const {return m_size;}

    Type type() const {return m_type;}
    QString name() const noexcept {return WindowFunction::TypeMap.at(m_type);}
    void setType(Type t);
    QVariant getTypes() const;

    float gain() const {return m_gain;}
    const float& get(unsigned int k) const { return m_data[k]; }

};
QDebug operator<<(QDebug dbg, const WindowFunction::Type &t);
#endif // WINDOWFUNCTION_H
