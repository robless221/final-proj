#include "student_code.h"
#include "mutablePriorityQueue.h"

using namespace std;

namespace CGL
{

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (class member).
   *
   * @param points A vector of points in 2D
   * @return A vector containing intermediate points or the final interpolated vector
   */
  std::vector<Vector2D> BezierCurve::evaluateStep(std::vector<Vector2D> const &points)
  {
    // TODO Part 1.
    std::vector<Vector2D> vector = std::vector<Vector2D>();
    for (int i = 0; i < points.size() - 1; i++) {
      vector.push_back((1 - t) * points.at(i) + t * points.at(i + 1));
    }
    return vector;
  }

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (function parameter).
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return A vector containing intermediate points or the final interpolated vector
   */
  std::vector<Vector3D> BezierPatch::evaluateStep(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    std::vector<Vector3D> vector = std::vector<Vector3D>();
    for (int i = 0; i < points.size() - 1; i++) {
      vector.push_back((1 - t) * points.at(i) + t * points.at(i + 1));
    }
    return vector;
  }

  /**
   * Fully evaluates de Casteljau's algorithm for a vector of points at scalar parameter t
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate1D(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    if (points.size() == 1) {
      return points.at(0);
    }
    return evaluate1D(evaluateStep(points, t), t);
  }

  /**
   * Evaluates the Bezier patch at parameter (u, v)
   *
   * @param u         Scalar interpolation parameter
   * @param v         Scalar interpolation parameter (along the other axis)
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate(double u, double v) const 
  {  
    // TODO Part 2.
    std::vector<Vector3D> vector = std::vector<Vector3D>();
    for (int i = 0; i < controlPoints.size(); i++) {
      vector.push_back(evaluate1D(controlPoints.at(i), u));
    }
    return evaluate1D(vector, v);
  }
 
  Vector3D Vertex::normal( void ) const
  {
    // TODO Part 3.
    // Returns an approximate unit normal at this vertex, computed by
    // taking the area-weighted average of the normals of neighboring
    // triangles, then normalizing.
    Vector3D normalVector = Vector3D();
    HalfedgeCIter h = halfedge();
    
    do {
      HalfedgeCIter twin = h->twin();
      Vector3D p1 = h->vertex()->position;
      Vector3D p2 = twin->vertex()->position;
      Vector3D p3 = twin->next()->next()->vertex()->position;
      
      Vector3D u = p2 + (-1 * p1);
      Vector3D v = p3 + (-1 * p1);
      Vector3D normal = cross(v, u);
      
      normalVector += normal;
      h = twin->next();
        
    } while (h != halfedge());
      
    normalVector.normalize();
    return normalVector;
  }

  EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
  {
    // TODO Part 4.
    // This method should flip the given edge and return an iterator to the flipped edge.
    if (e0->halfedge()->isBoundary()) {
      return e0;
    }
      
      HalfedgeIter h1 = e0->halfedge();
      HalfedgeIter h1_next = h1->next();
      HalfedgeIter h1_next_twin = h1_next->twin();
      HalfedgeIter h1_next_next = h1_next->next();
      HalfedgeIter h1_next_next_twin = h1_next_next->twin();
      HalfedgeIter h2 = h1->twin();
      HalfedgeIter h2_next = h2->next();
      HalfedgeIter h2_next_twin = h2_next->twin();
      HalfedgeIter h2_next_next = h2_next->next();
      HalfedgeIter h2_next_next_twin = h2_next_next->twin();
      
      EdgeIter e1 = h1->edge();
      EdgeIter e2 = h1_next_next->edge();
      EdgeIter e3 = h2_next->edge();
      EdgeIter e4 = h2->edge();
      EdgeIter e5 = h2_next_next->edge();
      EdgeIter e6 = h1_next->edge();
      
      VertexIter a = h2_next_next->vertex();
      VertexIter b = h1_next->vertex();
      VertexIter c = h1->vertex();
      VertexIter d = h1_next_next->vertex();

      FaceIter f1 = h1->face();
      FaceIter f2 = h2->face();
      
      h1->setNeighbors(h1_next, h2, a, e1, f1);
      h1_next->setNeighbors(h1_next_next, h1_next_next_twin, d, e2, f1);
      h1_next_next->setNeighbors(h1, h2_next_twin, c, e3, f1);
      h2->setNeighbors(h2_next, h1, d, e4, f2);
      h2_next->setNeighbors(h2_next_next, h2_next_next_twin, a, e5, f2);
      h2_next_next->setNeighbors(h2, h1_next_twin, b, e6, f2);
      h1_next_next_twin->setNeighbors(h1_next_next_twin->next(), h1_next, c, e2, h1_next_next_twin->face());
      h1_next_twin->setNeighbors(h1_next_twin->next(), h2_next_next, d, e6, h1_next_twin->face());
      h2_next_twin->setNeighbors(h2_next_twin->next(), h1_next_next, a, e3, h2_next_twin->face());
      h2_next_next_twin->setNeighbors(h2_next_next_twin->next(), h2_next, b, e5, h2_next_next_twin->face());

      e1->halfedge() = h1;
      e2->halfedge() = h1_next_next_twin;
      e3->halfedge() = h2_next_twin;
      e4->halfedge() = h2;
      e5->halfedge() = h2_next_next_twin;
      e6->halfedge() = h1_next_twin;
      
      a->halfedge() = h1;
      b->halfedge() = h2_next_next;
      c->halfedge() = h1_next_next;
      d->halfedge() = h1_next;
    
    return e0;
  }

  VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
  {
    return newVertex();
  }

  void HalfedgeMesh::addCentroid( FaceIter f0 )
  {
    HalfedgeIter h1 = f0->halfedge();
    HalfedgeIter h2 = h1->next();
    HalfedgeIter h3 = h2->next();
  
    if (h1->edge()->isBoundary() || h2->edge()->isBoundary() || h3->edge()->isBoundary()) {
      return;
    }
      
    VertexIter centroid = newVertex();
    centroid->position = 1.0/3.0 * (h1->vertex()->position + h2->vertex()->position + h3->vertex()->position);
    
    EdgeIter e1 = newEdge();
    EdgeIter e2 = newEdge();
    EdgeIter e3 = newEdge();

    FaceIter f1 = newFace();
    FaceIter f2 = newFace();
    
    HalfedgeIter a = newHalfedge();
    HalfedgeIter b = newHalfedge();
    HalfedgeIter c = newHalfedge();
    HalfedgeIter a2 = newHalfedge();
    HalfedgeIter b2 = newHalfedge();
    HalfedgeIter c2 = newHalfedge();
    
    VertexIter h1v = h1->vertex();
    VertexIter h2v = h2->vertex();
    VertexIter h3v = h3->vertex();

    h1->setNeighbors(a, h1->twin(), h1->vertex(), h1->edge(), f0);
    a->setNeighbors(b, a2, h2v, e1, f0);
    b->setNeighbors(h1, b2, centroid, e2, f0);
    
    h2->setNeighbors(c2, h2->twin(), h2->vertex(), h2->edge(), f1);
    c2->setNeighbors(a2, c, h3v, e3, f1);
    a2->setNeighbors(h2, a, centroid, e1, f1);
    
    h3->setNeighbors(b2, h3->twin(), h3->vertex(), h3->edge(), f2);
    b2->setNeighbors(c, b, h1v, e2, f2);
    c->setNeighbors(h3, c2, centroid, e3, f2);

    f0->halfedge() = h1;
    f1->halfedge() = h2;
    f2->halfedge() = h3;
    
    e1->halfedge() = a;
    e2->halfedge() = b;
    e3->halfedge() = c;
    centroid->halfedge() = a;
    
    e1->isNew = true;
    e2->isNew = true;
    e3->isNew = true;
    h1->edge()->isNew = false;
    h2->edge()->isNew = false;
    h3->edge()->isNew = false;
    
    h1v->halfedge() = h1;
    h2v->halfedge() = h2;
    h3v->halfedge() = h3;
    
    h1v->isNew = false;
    h2v->isNew = false;
    h3v->isNew = false;
    centroid->isNew = true;
  }

  // Square Root 3 Subdivsion
  void MeshResampler::upsample( HalfedgeMesh& mesh )
  {
    int totalFaces = mesh.nFaces();
    FaceIter face = mesh.facesBegin();
    for (int i = 0; i < totalFaces; i++) {
      FaceIter next = face;
      next++;
      mesh.addCentroid(face);
      face = next;
    }
  
    EdgeIter edge = mesh.edgesBegin();
    while (edge != mesh.edgesEnd()) {
      if (edge->isBoundary()) {
        continue;
      }
      EdgeIter next = edge;
      next++;
      if (!edge->isNew) {
        EdgeIter tmp = mesh.flipEdge(edge);
      }
      edge = next;
    }
    
    VertexIter vert = mesh.verticesBegin();
    while (vert != mesh.verticesEnd()) {
      VertexIter next = vert;
      next++;
      double n = 0.0;
      HalfedgeIter halfedge = vert->halfedge();
      HalfedgeIter end = vert->halfedge();
      Vector3D pos = Vector3D(0,0,0);
      do {
        Vector3D position = halfedge->twin()->vertex()->position;
        pos += position;
        halfedge = halfedge->twin()->next();
        n += 1;
      } while (halfedge != end);
      double betta = (4.0 - 2.0 * cos(2 * M_PI / n)) / 9.0 / n;
      vert->position = (1.0 - n * betta) * vert->position + betta * pos;
      vert = next;
    }
  }
}
