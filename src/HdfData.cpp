/*!
 * Implementation of HdfData; non templated functions.
 */

#include "HdfData.h"
#include "tools.h"
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>

using std::vector;
using std::string;
using std::runtime_error;
using std::stringstream;
using std::pair;
using std::make_pair;

#include <iostream>
using std::cout;
using std::endl;

#include <opencv2/opencv.hpp>

morph::HdfData::HdfData (const string fname, const bool read_data)
{
    this->read_mode = read_data;
    if (this->read_mode == true) {
        this->file_id = H5Fopen (fname.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    } else {
        this->file_id = H5Fcreate (fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    }
    // Check it's open, if not throw exception
    if ((int)this->file_id < 0) {
        stringstream ee;
        ee << "Error opening HDF5 file '" << fname << "'";
        throw runtime_error (ee.str());
    }
}

morph::HdfData::~HdfData()
{
    herr_t status = H5Fclose (this->file_id);
    if (status) {
        //stringstream ee;
        //ee << "Error closing HDF5 file; status: " << status;
        //throw runtime_error (ee.str());
    }
}

void
morph::HdfData::handle_error (const herr_t& status, const string& emsg)
{
    if (status) {
        stringstream ee;
        ee << emsg << status;
        throw runtime_error (ee.str());
    }
}

/*!
 * read_contained_vals() overloads
 */
//@{
void
morph::HdfData::read_contained_vals (const char* path, vector<double>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    // Get number of elements in the dataset at path, and resize vals
    // so it's ready to receive the data.
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    // This line is different in the overloaded implementations:
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<float>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<array<float, 3>>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    if (dims[1] != 3) {
        stringstream ee;
        ee << "Error. Expected 3 coordinates to be stored in each element of " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0]);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<array<float, 12>>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    if (dims[1] != 12) {
        stringstream ee;
        ee << "Error. Expected 12 floats to be stored in each element of " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0]);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<cv::Point2i>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    if (dims[1] != 2) {
        stringstream ee;
        ee << "Error. Expected 2 coordinates to be stored in each cv::Point of " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0]);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<cv::Point2d>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    if (dims[1] != 2) {
        stringstream ee;
        ee << "Error. Expected 2 coordinates to be stored in each cv::Point of " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0]);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<cv::Point2f>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    if (dims[1] != 2) {
        stringstream ee;
        ee << "Error. Expected 2 coordinates to be stored in each cv::Point of " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0]);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, cv::Mat& vals)
{
    // First get the type metadata
    string pathtype (path);
    pathtype += "_type";
    int cv_type = 0;
    this->read_val (pathtype.c_str(), cv_type);
    pathtype = string(path) + "_channels";
    int channels = 0;
    this->read_val (pathtype.c_str(), channels);

    // Now read the matrix
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[2] = {0,0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 2) {
        stringstream ee;
        ee << "Error. Expected 2D data to be stored in " << path;
        throw runtime_error (ee.str());
    }

    // Dims gives size in absolute number of elements. If channels > 1, then the Mat
    // needs to be resized accordingly
    int matcols = dims[1] / channels;

    // Resize the Mat
    vals.create ((int)dims[0], matcols, cv_type);

    herr_t status;
    switch (cv_type) {
    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_8SC1:
    case CV_8SC2:
    case CV_8SC3:
    case CV_8SC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_16SC1:
    case CV_16SC2:
    case CV_16SC3:
    case CV_16SC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_32SC1:
    case CV_32SC2:
    case CV_32SC3:
    case CV_32SC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_64FC1:
    case CV_64FC2:
    case CV_64FC3:
    case CV_64FC4:
    {
        status = H5Dread (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    default:
    {
        //cerr << "Unknown CvType " << cv_type << endl;
        status = -1; // What's correct for an error here?
        break;
    }
    }
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<int>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<unsigned int>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<long long int>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, vector<unsigned long long int>& vals)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected 1D data to be stored in " << path;
        throw runtime_error (ee.str());
    }
    vals.resize (dims[0], 0.0);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_ULLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_contained_vals (const char* path, pair<float, float>& vals)
{
    vector<float> vvals;
    this->read_contained_vals (path, vvals);
    if (vvals.size() != 2) {
        stringstream ee;
        ee << "Error. Expected pair<float, float> data to be stored in a vector of size 2";
        throw runtime_error (ee.str());
    }
    vals.first = vvals[0];
    vals.second = vvals[1];
}

void
morph::HdfData::read_contained_vals (const char* path, pair<double, double>& vals)
{
    vector<double> vvals;
    this->read_contained_vals (path, vvals);
    if (vvals.size() != 2) {
        stringstream ee;
        ee << "Error. Expected pair<double, double> data to be stored in a vector of size 2";
        throw runtime_error (ee.str());
    }
    vals.first = vvals[0];
    vals.second = vvals[1];
}

void
morph::HdfData::read_contained_vals (const char* path, list<pair<float, float>>& vals)
{
    string p1(path);
    p1 += "_first";
    string p2(path);
    p2 += "_second";

    vector<float> first;
    this->read_contained_vals (p1.c_str(), first);
    vector<float> second;
    this->read_contained_vals (p2.c_str(), second);

    if (first.size() != second.size()) {
        stringstream ee;
        ee << "Error. Expected two vectors *_first and *_second of same length.";
        throw runtime_error (ee.str());
    }

    vals.clear();
    for (unsigned int i = 0; i < first.size(); ++i) {
        vals.push_back (make_pair (first[i], second[i]));
    }
}

void
morph::HdfData::read_contained_vals (const char* path, list<pair<double, double>>& vals)
{
    string p1(path);
    p1 += "_first";
    string p2(path);
    p2 += "_second";

    vector<double> first;
    this->read_contained_vals (p1.c_str(), first);
    vector<double> second;
    this->read_contained_vals (p2.c_str(), second);

    if (first.size() != second.size()) {
        stringstream ee;
        ee << "Error. Expected two vectors *_first and *_second of same length.";
        throw runtime_error (ee.str());
    }

    vals.clear();
    for (unsigned int i = 0; i < first.size(); ++i) {
        vals.push_back (make_pair (first[i], second[i]));
    }
}

void
morph::HdfData::read_val (const char* path, double& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, float& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, int& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, unsigned int& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, long long int& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, unsigned long long int& val)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_ULLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}

void
morph::HdfData::read_val (const char* path, bool& val)
{
    unsigned int uival = 0;
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    herr_t status = H5Dread (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &uival);
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    // Copy unsigned int value into bool:
    val = uival > 0 ? true : false;
}
//@}

void
morph::HdfData::process_groups (const char* path)
{
    vector<string> pbits = morph::Tools::stringToVector (path, "/");
    unsigned int numgroups = pbits.size() - 1;
    if (numgroups > 1) { // There's always the first, empty (root) group
        string groupstr("");
        for (unsigned int g = 1; g < numgroups; ++g) {
            groupstr += "/" + pbits[g];
            this->verify_group (groupstr);
        }
    }
}

void
morph::HdfData::verify_group (const string& path)
{
    if (H5Lexists (this->file_id, path.c_str(), H5P_DEFAULT) <= 0) {
        //cout << "Create group " << path << endl;
        hid_t group = H5Gcreate (this->file_id, path.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        herr_t status = H5Gclose (group);
        this->handle_error (status, "Error. status after H5Gclose: ");
    }
}

/*!
 * add_val() overloads
 */
//@{
void
morph::HdfData::add_val (const char* path, const double& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    // Try hsize_t dim_singleparam[1] = {1} later...

    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    // NB: Always use H5T_IEEE_F64LE to save the data in the file, so this line doesn't need specialisation:
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // This line is really the only difference between
    // add_fpoint(const char*, const float&) and add_fpoint(const
    // char*, const double&)
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const float& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const int& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const unsigned int& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const long long int& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const unsigned long long int& val)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_ULLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &val);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_val (const char* path, const bool& val)
{
    unsigned int uival = 0;
    if (val == true) {
        uival = 1;
    }
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = 1;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &uival);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}
//@} // add_val overloads

/*!
 * add_ptrarray_vals() overloads
 */
//@{
void
morph::HdfData::add_ptrarray_vals (const char* path, double*& vals, const unsigned int nvals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = nvals;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_ptrarray_vals (const char* path, float*& vals, const unsigned int nvals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = nvals;
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals);
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}
//@}

/*!
 * add_contained_vals() overloads
 */
//@{
void
morph::HdfData::add_contained_vals (const char* path, const vector<double>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<float>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const list<pair<float, float>>& vals)
{
    // A list of pairs is two cols. Write into two vectors, first and second, then
    // add_contained_vals from that.
    vector<float> first (vals.size(), 0.0f);
    vector<float> second (vals.size(), 0.0f);
    unsigned int i = 0;
    for (pair<float, float> p : vals) {
        first[i] = p.first;
        second[i] = p.second;
        ++i;
    }
    string p1(path);
    p1 += "_first";
    string p2(path);
    p2 += "_second";
    this->add_contained_vals (p1.c_str(), first);
    this->add_contained_vals (p2.c_str(), second);
}

void
morph::HdfData::add_contained_vals (const char* path, const list<pair<double, double>>& vals)
{
    // A list of pairs is two cols. Write into two vectors, first and second, then
    // add_contained_vals from that.
    vector<double> first (vals.size(), 0.0f);
    vector<double> second (vals.size(), 0.0f);
    unsigned int i = 0;
    for (pair<double, double> p : vals) {
        first[i] = p.first;
        second[i] = p.second;
        ++i;
    }
    string p1(path);
    p1 += "_first";
    string p2(path);
    p2 += "_second";
    this->add_contained_vals (p1.c_str(), first);
    this->add_contained_vals (p2.c_str(), second);
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<array<float, 3>>& vals)
{
    this->process_groups (path);
    hsize_t dim_vec3dcoords[2]; // 2 Dims
    dim_vec3dcoords[0] = vals.size();
    dim_vec3dcoords[1] = 3; // 3 floats in each array<float,3>
    // Note 2 dims (1st arg, which is rank = 2)
    hid_t dataspace_id = H5Screate_simple (2, dim_vec3dcoords, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<array<float, 12>>& vals)
{
    this->process_groups (path);
    hsize_t dim_vec12f[2];
    dim_vec12f[0] = vals.size();
    dim_vec12f[1] = 12;
    hid_t dataspace_id = H5Screate_simple (2, dim_vec12f, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<cv::Point2i>& vals)
{
    this->process_groups (path);
    hsize_t dim_vec2dcoords[2]; // 2 Dims
    dim_vec2dcoords[0] = vals.size();
    dim_vec2dcoords[1] = 2; // 2 ints in each cv::Point
    // Note 2 dims (1st arg, which is rank = 2)
    hid_t dataspace_id = H5Screate_simple (2, dim_vec2dcoords, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<cv::Point2d>& vals)
{
    this->process_groups (path);
    hsize_t dim_vec2dcoords[2]; // 2 Dims
    dim_vec2dcoords[0] = vals.size();
    dim_vec2dcoords[1] = 2; // 2 doubles in each cv::Point2d
    // Note 2 dims (1st arg, which is rank = 2)
    hid_t dataspace_id = H5Screate_simple (2, dim_vec2dcoords, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<cv::Point2f>& vals)
{
    this->process_groups (path);
    hsize_t dim_vec2dcoords[2]; // 2 Dims
    dim_vec2dcoords[0] = vals.size();
    dim_vec2dcoords[1] = 2; // 2 doubles in each cv::Point2d
    // Note 2 dims (1st arg, which is rank = 2)
    hid_t dataspace_id = H5Screate_simple (2, dim_vec2dcoords, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const cv::Mat& vals)
{
    this->process_groups (path);

    hsize_t dim_mat[2]; // 2 dimensions supported (even though Mat's can do n dimensions)

    cv::Size ms = vals.size();
    dim_mat[0] = ms.height;
    int channels = vals.channels();
    if (channels > 4) {
        // error, can't handle >4 channels
    }
    dim_mat[1] = ms.width * channels;

    hid_t dataspace_id = H5Screate_simple (2, dim_mat, NULL);

    hid_t dataset_id;
    herr_t status;

    int cv_type = vals.type();

    switch (cv_type) {

    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U8LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_8SC1:
    case CV_8SC2:
    case CV_8SC3:
    case CV_8SC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I8LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U16LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_16SC1:
    case CV_16SC2:
    case CV_16SC3:
    case CV_16SC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I16LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_32SC1:
    case CV_32SC2:
    case CV_32SC3:
    case CV_32SC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I32LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F32LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    case CV_64FC1:
    case CV_64FC2:
    case CV_64FC3:
    case CV_64FC4:
    {
        dataset_id = H5Dcreate2 (this->file_id, path, H5T_IEEE_F64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vals.data);
        break;
    }
    default:
    {
        //cerr << "Unknown CvType " << cv_type << endl;
        dataset_id = -1; // What's correct for an error here?
        break;
    }
    }

    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");

    // Last, write the type in a special metadata field.
    string pathtype (path);
    pathtype += "_type";
    this->add_val (pathtype.c_str(), cv_type);
    pathtype = string(path) + "_channels";
    this->add_val (pathtype.c_str(), channels);
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<int>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<unsigned int>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<long long int>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_I64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const vector<unsigned long long int>& vals)
{
    this->process_groups (path);
    hsize_t dim_singleparam[1];
    dim_singleparam[0] = vals.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singleparam, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_STD_U64LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_NATIVE_ULLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(vals[0]));
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::add_contained_vals (const char* path, const pair<float, float>& vals)
{
    vector<float> vf;
    vf.push_back (vals.first);
    vf.push_back (vals.second);
    this->add_contained_vals (path, vf);
}

void
morph::HdfData::add_contained_vals (const char* path, const pair<double, double>& vals)
{
    vector<double> vf;
    vf.push_back (vals.first);
    vf.push_back (vals.second);
    this->add_contained_vals (path, vf);
}
//@} // add_contained_vals overloads

/*!
 * String saving
 */
//@{
void
morph::HdfData::add_string (const char* path, const string& str)
{
    this->process_groups (path);
    hsize_t dim_singlestring[1];
    dim_singlestring[0] = str.size();
    hid_t dataspace_id = H5Screate_simple (1, dim_singlestring, NULL);
    hid_t dataset_id = H5Dcreate2 (this->file_id, path, H5T_C_S1, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite (dataset_id, H5T_C_S1, H5S_ALL, H5S_ALL, H5P_DEFAULT, str.c_str());
    this->handle_error (status, "Error. status after H5Dwrite: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
    status = H5Sclose (dataspace_id);
    this->handle_error (status, "Error. status after H5Sclose: ");
}

void
morph::HdfData::read_string (const char* path, string& str)
{
    hid_t dataset_id = H5Dopen2 (this->file_id, path, H5P_DEFAULT);
    hid_t space_id = H5Dget_space (dataset_id);
    hsize_t dims[1] = {0};
    int ndims = H5Sget_simple_extent_dims (space_id, dims, NULL);
    if (ndims != 1) {
        stringstream ee;
        ee << "Error. Expected string to be stored as 1D data in " << path;
        throw runtime_error (ee.str());
    }
    str.resize (dims[0], ' ');
    herr_t status = H5Dread (dataset_id, H5T_C_S1, H5S_ALL, H5S_ALL, H5P_DEFAULT, &(str[0]));
    this->handle_error (status, "Error. status after H5Dread: ");
    status = H5Dclose (dataset_id);
    this->handle_error (status, "Error. status after H5Dclose: ");
}
//@}
