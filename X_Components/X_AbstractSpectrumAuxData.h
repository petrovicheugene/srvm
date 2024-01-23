//================================================
#ifndef X_ABSTRACTSPECTRUMAUXDATA_H
#define X_ABSTRACTSPECTRUMAUXDATA_H
//================================================

//================================================
class X_AbstractSpectrumAuxData
{
public:
    enum DataType {DT_SPE};
    explicit X_AbstractSpectrumAuxData(DataType type);

    DataType xp_type() const;

private:

    // VARS

    DataType xv_type;

};
//================================================
#endif // X_ABSTRACTSPECTRUMAUXDATA_H
