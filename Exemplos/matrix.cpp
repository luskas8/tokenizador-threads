#include <iostream>

int main(int argc, char *argv[]) {

    int n;
    float **m_a, **m_b, **m_c;

    if(argc < 2) return -1;
    n = atoi(argv[1]);
    
    m_a = new float*[n];
    m_b = new float*[n];
    m_c = new float*[n];
    for(int i=0; i<n; i++)
    {
        m_a[i] = new float[n];
        m_b[i] = new float[n];
        m_c[i] = new float[n];
    }

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
            m_a[i][j] = i+1;
            m_b[i][j] = (i+1) + (j+1);
            m_c[i][j] = 0.0;
        }

    for (int i=0; i<n; i++)
        for ( int j=0; j<n; j++) 
        {
            m_c[i][j] = 0.0;
            for ( int k=0; k<n; k++)
                m_c[i][j] = m_c[i][j] + m_a[i][k] * m_b[k][j];
        }

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            std::cout << m_c[i][j] << ", ";

        std::cout << std::endl;
    };

    return 0;
}
