//================================================
#ifndef ZABSTRACTSPECTRUMAUXDATA_H
#define ZABSTRACTSPECTRUMAUXDATA_H
//================================================

//================================================
class ZAbstractSpectrumAuxData
{
public:
    enum DataType {DT_SPE};
    ZAbstractSpectrumAuxData(DataType type);

    DataType zp_type();

private:

    // VARS

    DataType zv_type;

};
//================================================
#endif // ZABSTRACTSPECTRUMAUXDATA_H
