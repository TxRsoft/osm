/**
 *  OSM
 *  Copyright (C) 2019  Pavel Smokotnin

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
#ifndef FREQUENCYBASEDSERIESRENDERER_H
#define FREQUENCYBASEDSERIESRENDERER_H

#include "xyseriesrenderer.h"
namespace Fftchart {

class FrequencyBasedSeriesRenderer : public XYSeriesRenderer
{

protected:
    void setUniforms();
    void iterate(unsigned int pointsPerOctave,
                const std::function<void(unsigned int)> &accumulate,
                const std::function<void(float start, float end, unsigned int count)> &collected
            );

    template<typename T, typename TSpline = T> void iterateForSpline(unsigned int pointsPerOctave,
                T *value, float *coherence,
                const std::function<void (unsigned int)> &accumulate,
                const std::function<void(float f1, float f2, TSpline *a, GLfloat *c)> &collected,
                const std::function<TSpline(const T* value, const float *f, const unsigned int & index)> &beforeSpline = {}
            )
    {
        bool bCollected = false;
        unsigned int bCount = 0;
        TSpline splinePoint[4], a[4];
        float csplinePoint[4], f[4], c[4];

        auto it = [value, coherence, &collected, &splinePoint, &csplinePoint,
                    &f, &a, &c, &bCollected, &beforeSpline, &bCount]
                (float bandStart, float bandEnd, unsigned int count)
        {
            Q_UNUSED(bandStart)

            *coherence /= count;

            if (bCollected) {
                splinePoint[0]  = splinePoint[1];f[0] = f[1];
                splinePoint[1]  = splinePoint[2];f[1] = f[2];
                splinePoint[2]  = splinePoint[3];f[2] = f[3];

                csplinePoint[0]  = csplinePoint[1];
                csplinePoint[1]  = csplinePoint[2];
                csplinePoint[2]  = csplinePoint[3];
            }
            f[bCount] = (bandStart + bandEnd) / 2.f;
            if (beforeSpline) {
                splinePoint[bCount] = beforeSpline(value, f, bCount) / count;
            } else if constexpr (std::is_same<T, TSpline>::value) {
                splinePoint[bCount] = (*value) / count;
            } else {
                qDebug() << "could not convert TSpline to T";
            }
            csplinePoint[bCount] = *coherence;

            if (bCount == 3) {
                a[0] = (splinePoint[0]      + splinePoint[1] * 4 + splinePoint[2]) / 6;
                a[1] = (splinePoint[0] * -1 +                      splinePoint[2]) / 2;
                a[2] = (splinePoint[0]      - splinePoint[1] * 2 + splinePoint[2]) / 2;
                a[3] = (splinePoint[0] * -1 + splinePoint[1] * 3 - splinePoint[2] * 3 + splinePoint[3]) / 6;

                c[0] = (     csplinePoint[0] + 4 * csplinePoint[1] +     csplinePoint[2]) / 6;
                c[1] = (-1 * csplinePoint[0] +                           csplinePoint[2]) / 2;
                c[2] = (     csplinePoint[0] - 2 * csplinePoint[1] +     csplinePoint[2]) / 2;
                c[3] = (-1 * csplinePoint[0] + 3 * csplinePoint[1] - 3 * csplinePoint[2] + csplinePoint[3]) / 6;

                bCollected = true;
                collected(f[1], f[2], a, c);
            } else {
                ++bCount;
            }
        };
        iterate(pointsPerOctave, accumulate, it);
    }

public:
    explicit FrequencyBasedSeriesRenderer();
    virtual void renderSeries() = 0;

protected:
    int m_matrixUniform,
        m_minmaxUniform,
        m_screenUniform,
        m_widthUniform;
};
}
#endif // FREQUENCYBASEDSERIESRENDERER_H
