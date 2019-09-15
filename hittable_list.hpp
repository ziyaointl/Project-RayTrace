#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP
#include <vector>

class HittableList: public Hittable {
    public:
        HittableList() = default;
        std::vector<Hittable*> hittables;
        virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
            bool hitAnything = false;
            HitRecord tempRec;
            for (Hittable* hittable : hittables) {
                if (hittable->hit(r, tMin, tMax, tempRec)) {
                    hitAnything = true;
                    tMax = tempRec.t;
                    rec = tempRec;
                }
            }
            return hitAnything;
        }
};

#endif
