#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include <vector>

namespace dxlib {

class DBHelper
{
  public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 更新一行记录里的一个值. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="db">              [in]要操作的数据库. </param>
    /// <param name="table_name">      要修改的表名. </param>
    /// <param name="where_col_name">  选择判断的列名. </param>
    /// <param name="where_row_value"> 选择判断的行的值. </param>
    /// <param name="update_col_name"> 要更新的列名. </param>
    /// <param name="update_value">    要更新的值. </param>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    static int UPDATE(SQLite::Database* db, const std::string& table_name,
                      const std::string& where_col_name, const std::string& where_row_value,
                      const std::string& update_col_name, const std::string& update_value);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 查询一项信息(字符串). </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="db">              [in]要操作的数据库.. </param>
    /// <param name="table_name">      要查询的表名. </param>
    /// <param name="where_col_name">  选择判断的列名. </param>
    /// <param name="where_row_value"> 选择判断的行的值. </param>
    /// <param name="select_col_name"> 要查询的列名. </param>
    /// <param name="value">           [out]查询结果. </param>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    static int SELECT(SQLite::Database* db, const std::string& table_name,
                      const std::string& where_col_name, const std::string& where_row_value,
                      const std::string& select_col_name, std::string& value);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 查询一项信息(整型). </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="db">              [in]要操作的数据库.. </param>
    /// <param name="table_name">      要查询的表名. </param>
    /// <param name="where_col_name">  选择判断的列名. </param>
    /// <param name="where_row_value"> 选择判断的行的值. </param>
    /// <param name="select_col_name"> 要查询的列名. </param>
    /// <param name="value">           [out]查询结果. </param>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    static int SELECT(SQLite::Database* db, const std::string& table_name,
                      const std::string& where_col_name, const std::string& where_row_value,
                      const std::string& select_col_name, unsigned int& value);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 查询一项信息(二进制数据). </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="db">              [in]要操作的数据库.. </param>
    /// <param name="table_name">      要查询的表名. </param>
    /// <param name="where_col_name">  选择判断的列名. </param>
    /// <param name="where_row_value"> 选择判断的行的值. </param>
    /// <param name="select_col_name"> 要查询的列名. </param>
    /// <param name="value">           [out]查询结果. </param>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    static int SELECT(SQLite::Database* db, const std::string& table_name,
                      const std::string& where_col_name, const std::string& where_row_value,
                      const std::string& select_col_name, std::vector<char>& value);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Compress. </summary>
    ///
    /// <remarks>
    /// 实际上关于这个压缩，如果第一次压缩的压缩率不高，那么可以尝试第二次压缩.
    ///     Dx, 2019/8/5.
    /// </remarks>
    ///
    /// <param name="data">         原始数据. </param>
    /// <param name="compressData"> [out] 压缩后的数据. </param>
    ///-------------------------------------------------------------------------------------------------
    static void compress(const std::vector<char>& data, std::vector<char>& compressData);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Compress. </summary>
    ///
    /// <remarks> Dx, 2019/8/6. </remarks>
    ///
    /// <param name="data">         原始数据起始. </param>
    /// <param name="byteSize">     数据的字节大小. </param>
    /// <param name="compressData"> [out] 压缩后的数据. </param>
    ///-------------------------------------------------------------------------------------------------
    static void compress(const void* data, size_t byteSize, std::vector<char>& compressData);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Compress. </summary>
    ///
    /// <remarks> Dx, 2019/8/6. </remarks>
    ///
    /// <typeparam name="T"> Generic type parameter. </typeparam>
    /// <param name="data">         原始数据. </param>
    /// <param name="compressData"> [out] 压缩后的数据. </param>
    ///-------------------------------------------------------------------------------------------------
    template <typename T>
    static void compress(const std::vector<T>& data, std::vector<char>& compressData)
    {
        compress(data.data(), data.size() * sizeof(T), compressData);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Decompress. </summary>
    ///
    /// <remarks> Dx, 2019/8/5. </remarks>
    ///
    /// <param name="compressData">   压缩数据. </param>
    /// <param name="decompressData"> [out] 解压缩后的数据. </param>
    ///-------------------------------------------------------------------------------------------------
    static void decompress(const std::vector<char>& compressData, std::vector<char>& decompressData);
};

} // namespace dxlib