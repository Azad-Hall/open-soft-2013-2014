#ifndef __CONSTANTS_H_INCLUDED__
#define __CONSTANTS_H_INCLUDED__

enum Constants
{
	IS_DEGREE = true
};

#endif

							pArray[0] = p[0];
							pArray[1] = orientedPolygon;
							//translate rotate
							rotError = rotError/errorCount;
							for(int i = 0; i < orientedPolygon->nVertices; i++)
							{
								//orientedPolygon->vertex[i].x = orientedPolygon->vertex[i].x + error.x/errorCount;
								//orientedPolygon->vertex[i].y = orientedPolygon->vertex[i].y + error.y/errorCount;
								shift.x = orientedPolygon->vertex[i].x - orientedPolygon->vertex[start1].x;
								shift.y = orientedPolygon->vertex[i].y - orientedPolygon->vertex[start1].y;
								rot.x = shift.x * cos(rotError) + shift.y * sin(rotError);
								rot.y = -shift.x * sin(rotError) + shift.y * cos(rotError);
								orientedPolygon->vertex[i].x = rot.x + orientedPolygon->vertex[start1].x;
								orientedPolygon->vertex[i].y = rot.y + orientedPolygon->vertex[start1].y;
							}