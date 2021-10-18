#include<iostream>
#include<chrono>
#include<utility>
class QU {
private:
    int N;
    std::pair<int, int>* id;
    int count;
public:
    QU(int n) :N(n) {
        id = new std::pair<int, int>[n];
        for (int i = 0; i < n; i++)
            id[i] = { i,1 };
        count = n;
    };
    int root(int a)
    {
        int t = id[a].first;
        while (t != id[t].first) {
            id[t] = id[id[t].first];//path compression by pointing
            //its parent to its grandparent all they way up to the root
            t = id[t].first;
        }
        return t;
    }
    int connected(int a, int b)
    {
        return root(a) == root(b);
    }
    void unite(int p, int q)
    {
        int rootP = root(p);
        int rootQ = root(q);
        if (rootP == rootQ) return;
        if (id[rootP].second < id[rootQ].second) {
            id[rootP].first = rootQ;
            id[rootQ].second += id[rootP].second;
        }
        else {
            id[rootQ].first = rootP;
            id[rootP].second += id[rootQ].second;
        }
        count--;
    }
    void print() {
        for (int i = 0; i < N; i++)
        {
            std::cout << id[i].first << '-' << id[i].second << ' ';
        }
        std::cout << std::endl;
    }
    int noOfComponents()
    {
        return count;
    }
    ~QU() {
        delete[] id;
    }
};
struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		float ms = duration.count() * 1000.0f;
		std::cout << "Timer took " << ms << "ms" << std::endl;
	}

};
class Percolation {
private:
    int N; //width of grid
    double p=0.0;
    QU* qu;
    bool* grid;
public:
    Percolation(int n) :
        N(n) {
        qu = new QU(n * n + 2);
        for (int i = 0; i < n; i++)
            qu->unite(n * n, i);
        for (int i = n * n - 1; i >= n * n - n; i--)
            qu->unite(n * n + 1, i);
        grid = new bool[n * n];
        for (int i = 0; i < n * n; i++)
            grid[i] = false;
        srand(time(0));
        long long i = 0;
        long long n2 = n * n;
        long long trues = 0;
        for (; i < n2; i++) {
            int r = rand() % (n * n);
            if (!grid[r]) {
                grid[r] = true;
                trues++;
                if (qu->connected(n * n, n * n + 1)) break;
                if ((r + 1) % n<n && (r + 1) % n>r % n && grid[r + 1]) {
                    qu->unite(r, r + 1);
                }
                if ((r - 1) % n >= 0 && (r - 1) % n < r % n && grid[r - 1]) {
                    qu->unite(r - 1, r);
                }
                if (n + r< n * n && grid[n+r]) {
                    qu->unite(r, r + n);
                }
                if (r - n >= 0 && grid[r - n]) {
                    qu->unite(r - n, r);
                }
            }
        }
        p = (double)(trues-1)/ n2;
    }
    double getp() {
        return p;
    }
    ~Percolation() {
        delete[] grid;
        delete qu;
    }
};
void function(double sum, int n,int T) {
    Timer timer;
    for (int i = 0; i < T; i++) {
        Percolation pr(n);
        if (pr.getp() != 0.0) {
            sum += pr.getp();
        }
    }
    std::cout << T << "th average " << sum / (T) << "\n";
}
int main()
{
	int n,t;
	std::cin >> n>>t;
    double sum=0.0f;
    function(sum, n,t);
}