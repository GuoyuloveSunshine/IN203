// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>
# include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>{
private:
    int m_nrows, m_ncols,m_rank,m_nbp;
    std::vector<double> m_arr_coefs;
public:
    Matrix (int dim, int rank, int nbp);
    Matrix( int nrows, int ncols, int rank, int nbp);
    Matrix( const Matrix& A ) = delete;
    Matrix( Matrix&& A ) = default;
    ~Matrix() = default;

    Matrix& operator = ( const Matrix& A ) = delete;
    Matrix& operator = ( Matrix&& A ) = default;
    
    double& operator () ( int i, int j ) {
        return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () ( int i, int j ) const {
        return m_arr_coefs[i + j*m_nrows];
    }
    
    std::vector<double> operator * ( const std::vector<double>& u ) const;
    
    std::ostream& print( std::ostream& out ) const
    {
        const Matrix& A = *this;
        out << "[\n";
        for ( int i = 0; i < m_nrows; ++i ) {
            out << " [ ";
            for ( int j = 0; j < m_ncols; ++j ) {
                out << A(i,j) << " ";
            }
            out << " ]\n";
        }
        out << "]";
        return out;
    }
};
// ---------------------------------------------------------------------
inline std::ostream& 
operator << ( std::ostream& out, const Matrix& A )
{
    return A.print(out);
}
// ---------------------------------------------------------------------
inline std::ostream&
operator << ( std::ostream& out, const std::vector<double>& u )
{
    out << "[ ";
    for ( const auto& x : u )
        out << x << " ";
    out << " ]";
    return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * ( const std::vector<double>& u ) const{
    const Matrix& A = *this;
    int jstart = m_ncols * m_rank;
    std::vector<double> v(m_nrows, 0.);
    std::vector<double> v_loc(m_nrows, 0.);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v_loc[i] += A(i,j)*u[j + jstart];
        }
    }
    MPI_Allreduce(v_loc.data(), v.data(), m_nrows, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return v;
}

// =====================================================================
Matrix::Matrix (int dim, int rank, int nbp): m_nrows(dim), m_ncols(dim/nbp), m_rank(rank), m_nbp(nbp),
                           m_arr_coefs(dim*dim/nbp){
    int j_commence = rank * dim/nbp;
    for ( int i = 0; i < dim; ++ i ) { // i ligne
        for ( int j = 0; j < dim/nbp; ++j ) { // j colonne
            (*this)(i,j) = (i+j+j_commence)%dim;
        }
    }
}
// ---------------------------------------------------------------------
// Matrix::Matrix( int nrows, int ncols ) : m_nrows(nrows), m_ncols(ncols),
//                                          m_arr_coefs(nrows*ncols)
// {
//     int dim = (nrows > ncols ? nrows : ncols );
//     for ( int i = 0; i < nrows; ++ i ) {
//         for ( int j = 0; j < ncols; ++j ) {
//             (*this)(i,j) = (i+j)%dim;
//         }
//     }    
// }

// =====================================================================
int main( int nargs, char* argv[] ){
    const int N = 120;
    int rank, nbp;
    MPI_Init(&nargs, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbp);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(N, rank, nbp);
    std::cout  << "A["<< rank << "] : " << A << std::endl;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    std::cout << " u : " << u << std::endl;
    std::vector<double> v = A*u;
    std::cout << "A.u = " << v << std::endl;
    MPI_Finalize();
    return EXIT_SUCCESS;
}
