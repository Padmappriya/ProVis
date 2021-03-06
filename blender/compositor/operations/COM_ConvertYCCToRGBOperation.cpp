/*
 * Copyright 2011, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor:
 *		Dalai Felinto
 */

#include "COM_ConvertYCCToRGBOperation.h"
#include "BLI_math_color.h"

ConvertYCCToRGBOperation::ConvertYCCToRGBOperation() : NodeOperation()
{
	this->addInputSocket(COM_DT_COLOR);
	this->addOutputSocket(COM_DT_COLOR);
	this->m_inputOperation = NULL;
}

void ConvertYCCToRGBOperation::initExecution()
{
	this->m_inputOperation = this->getInputSocketReader(0);
}

void ConvertYCCToRGBOperation::setMode(int mode)
{
	switch (mode) {
		case 1:
			this->m_mode = BLI_YCC_ITU_BT709;
			break;
		case 2:
			this->m_mode = BLI_YCC_JFIF_0_255;
			break;
		case 0:
		default:
			this->m_mode = BLI_YCC_ITU_BT601;
			break;
	}
}

void ConvertYCCToRGBOperation::executePixel(float output[4], float x, float y, PixelSampler sampler)
{
	float inputColor[4];
	this->m_inputOperation->read(inputColor, x, y, sampler);

	/* need to un-normalize the data */
	/* R,G,B --> Y,Cb,Cr */
	mul_v3_fl(inputColor, 255.0f);

	ycc_to_rgb(inputColor[0], inputColor[1], inputColor[2], &output[0], &output[1], &output[2], this->m_mode);
	output[3] = inputColor[3];
}

void ConvertYCCToRGBOperation::deinitExecution()
{
	this->m_inputOperation = NULL;
}

