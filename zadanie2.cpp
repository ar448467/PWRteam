#include <bits/stdc++.h>

using namespace std;

// Small numerical tolerance
const double eps = 1e-9;

// 2D point structure
struct Point {
    double x, y;

    bool operator==(const Point& p) const {
        return (fabs(y - p.y) < eps && fabs(x - p.x) < eps);
    }

    bool operator<(const Point& p) const {
        if (fabs(x - p.x) > eps) return x < p.x;
        return y < p.y - eps;
    }
};

// Subtraction of two points 
Point operator-(const Point& a, const Point& b){
    return {a.x - b.x, a.y - b.y};
}

// Cross product of vectors OA × OB
double cross(const Point& O, const Point& A, const Point& B){
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Computes the convex hull using the Andrew's monotone chain algorithm
vector<Point> convex_hull(vector<Point> pts){
    if (pts.size() <= 2){
        return pts; 
    }

    // Sort points lexicographically (by x, then y)
    sort(pts.begin(), pts.end());

    int n = pts.size();
    vector<Point> hull(2 * n); // output array 
    int k = 0; // number of points currently in hull

    // lower hull
    for (int i = 0; i < n; i++){
        // Remove last point while the turn is not counter-clockwise
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= eps){
            k--;
        }
        hull[k] = pts[i]; // add current point
        k++;
    }

    // upper hull 
    int t = k + 1;
    for (int i = n - 2; i >= 0; i--){
        // Remove last point while the turn is not counter-clockwise
        while (k >= t && cross(hull[k - 2], hull[k - 1], pts[i]) <= eps){
            k--;
        }
        hull[k] = pts[i]; // add current point
        k++;
    }

    // Remove duplicate last point 
    hull.resize(k - 1);
    return hull;
}

// Returns vector length 
double length(const Point& v){
    return hypot(v.x, v.y);
}

// Returns distance between two points 
double dist(const Point& a, const Point& b){
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

// Computes the minimal width 
double min_width(const vector<Point>& hull){
    int n = (int)hull.size();
    if (n <= 2){
        return 0.0; // width is 0 for fewer than 3 points
    }

    double best = 0.0; 
    int j = 1; // antipodal point index

    // Iterate over all edges of the convex hull
    for (int i = 0; i < n; i++){
        // Compute current "height"
        double current = fabs(cross(hull[i], hull[(i + 1) % n], hull[j]));

        // Move j as long as the distance increases
        while (true){
            int jnext = (j + 1) % n;
            double candidate = fabs(cross(hull[i], hull[(i + 1) % n], hull[jnext]));
            if (candidate > current){
                j = jnext;
                current = candidate;
            } else {
                break; // found farthest point for this edge
            }
        }

        // Compute perpendicular distance
        double width = current / length(hull[(i + 1) % n] - hull[i]);

        // Update minimal width found
        if (i == 0 || width < best){
            best = width;
        }
    }
    return best;
}

// Comparator for sorting points by y, then by x
struct comparator {
    bool operator()(const Point& a, const Point& b) const {
        if (fabs(a.y - b.y) > eps) return a.y < b.y;
        return a.x < b.x;
    }
};

// Finds the closest pair of points using sweep line algorithm
tuple<Point, Point, double> closest_pair(vector<Point> pts){
    int n = (int)pts.size();
    if (n == 0){
        return {{0,0}, {0,0}, 0.0};
    }
    if (n == 1){
        return {pts[0], pts[0], 0.0};
    }

    // Sort all points by x coordinate
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){
        return a.x < b.x;
    });

    // Active set of points ordered by y
    set<Point, comparator> active_points;

    double min_dist = length(pts[1] - pts[0]) + 1.0;
    Point bestA = pts[0], bestB = pts[1];

    int first_in_window = 0; // first point within active "window" by x-distance

    // Sweep line from left to right
    for (int i = 0; i < n; ++i){
        Point current = pts[i];

        // Remove points too far away in x direction
        while (first_in_window < i && (current.x - pts[first_in_window].x) > min_dist){
            active_points.erase(pts[first_in_window]);
            ++first_in_window;
        }

        // Define y-range of potential candidates
        Point lowY  {current.x, current.y - min_dist};
        Point highY {current.x, current.y + min_dist};

        // Find all points in active set within that y-range
        auto lower = active_points.lower_bound(lowY);
        auto upper = active_points.upper_bound(highY);

        // Check only these few candidates
        for (auto it = lower; it != upper; ++it){
            double d = dist(current, *it);
            if (d < min_dist){
                min_dist = d;
                bestA = current;
                bestB = *it;
            }
        }

        // Add current point to active set
        active_points.insert(current);
    }
    return {bestA, bestB, min_dist};
}


// Prints the convex hull points in order
void print_convex_hull(vector<Point>& hull){
    cout << "Otoczka: ";
    for (size_t i = 0; i < hull.size(); ++i){
        if (i){
            cout<<", ";
        }
        cout<<'('<<hull[i].x <<", "<<hull[i].y<<')';
    }
    cout<<'\n';
}

// Prints the minimal strip width
void print_smallest_margin(double d){
    cout << fixed << setprecision(3);
    cout << "Proste: d=" << d << "\n";
}

// Prints the closest pair of points and their distance
void print_2_closest_points(Point a, Point b, double d){
    cout << fixed << setprecision(3);
    cout << "Najblizsze Punkty: [(" << a.x << ", " << a.y 
         << "), (" << b.x << ", " << b.y << ")] d=" << d << "\n";
}

// Global container for input points
vector<Point> pts;
// Reads point data from a text file into 'pts'
int load_data(const std::string& filename){
    ifstream in(filename);
    if (!in){
        cerr << "Nie mogę otworzyć pliku: " << filename << "\n";
        return 2; 
    }
    string line;
    int n;
    // Read number of points from the first line
    if (!std::getline(in, line) || !(std::stringstream(line) >> n)){
        cerr << "Brak liczby punktów\n";
        return 3; 
    }
    pts.reserve(n); 
    // Read all remaining lines as (x, y) coordinates
    while (getline(in, line)){
        stringstream ss(line);
        double x, y;

        if (!(ss >> x >> y)){
            cerr << "Błąd: " << line << "\n";
            return 4; 
        }

        pts.push_back({x, y});
    }

    in.close();
    return 0; // success
}

int main(int argc, char* argv[]){
    
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2){
        cerr << "Użycie: " << argv[0] << " <nazwa_pliku>\n";
        return 1;
    }
    string filename = argv[1];

    int ret = load_data(filename);
    if(ret) return ret;

    vector<Point> hull = convex_hull(pts);
    print_convex_hull(hull);

    double d = min_width(hull);
    print_smallest_margin(d);

    auto [a, b, d3] = closest_pair(pts);
    print_2_closest_points(a, b, d3);

    return 0;
}