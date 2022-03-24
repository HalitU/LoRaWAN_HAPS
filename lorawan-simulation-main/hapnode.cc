#include "hapnode.h" 

HAPNode::HAPNode()
{
    angleChange = 0.0;
}

// Data storage related functions
double HAPNode::GetDataMaxSize()
{
    return data_max_size;
}
double HAPNode::GetDataCurrentSize()
{
    return data_current_size;
}
double HAPNode::GetDataGenerationRate()
{
    return data_generation_rate;
}
double HAPNode::GetDataGenerationPeriod()
{
    return data_generation_period;
}
double HAPNode::GetDataWastedSize()
{
    return data_wasted_size;
}

std::string HAPNode::GetNodeType()
{
    return NodeType;
}

void HAPNode::SetNodeType(std::string nodeType)
{
    NodeType = nodeType;
}

void HAPNode::SetDataMaxSize(double newMaxSize)
{
    data_max_size = newMaxSize;
}
void HAPNode::SetDataCurrentSize(double newCurrentSize)
{
    data_current_size = newCurrentSize;
}
void HAPNode::SetDataGenerationRate(double newGenerationRate)
{
    data_generation_rate = newGenerationRate;
}
void HAPNode::SetDataGenerationPeriod(double newGenerationPeriod)
{
    data_generation_period = newGenerationPeriod;
}

void HAPNode::SetDataCollectionTime (double newDataCollectionTime)
{
    data_collection_time = newDataCollectionTime;
}

double HAPNode::GetDataCollectionTime ()
{
    return data_collection_time;
}

void HAPNode::IncrementCurrentSize()
{
    data_current_size += data_generation_rate;
}

void HAPNode::ResetSize()
{
    data_current_size = 0;
}

void HAPNode::DecrementCurrentSize(double sizeToRemove)
{
    if (data_current_size - sizeToRemove < 0)
    {
        data_current_size = 0;
    }
    else
    {
        data_current_size -= sizeToRemove;
    }
}

// Getters
double HAPNode::GetXVelocity()
{
    return X;
}
double HAPNode::GetYVelocity()
{
    return Y;
}

void HAPNode::SetXVelocity(double X_new)
{
    X = X_new;
}
void HAPNode::SetYVelocity(double Y_new)
{
    Y = Y_new;
}

// Data transmission records
void HAPNode::update_last_in_range_time (double current_time) {
    if (last_in_range_time < 0) {
        last_in_range_time = current_time;
    }
};
bool HAPNode::update_last_out_range_time (double current_time) {
    if (last_in_range_time >= 0 && last_out_range_time < 0) {
        last_out_range_time = current_time;
        return true;
    }
    return false;
};
void HAPNode::update_current_range_transmitted_data (double transmitted_data) {
    current_range_transmitted_data += transmitted_data;
    current_transmitted_count++;
};
void HAPNode::reset_last_in_range_time () {
    last_in_range_time = -1;
};
void HAPNode::reset_last_out_range_time () {
    last_out_range_time = -1;
};
void HAPNode::reset_current_range_transmitted_data () {
    current_range_transmitted_data = 0;
    current_transmitted_count = 0;
};
void HAPNode::write_latest_data_tx_record () {
    ofstream myfile;
    std::string fileName = GlobalVariables::experimentEnddeviceDataRecordFolder + "enddevice_" + std::to_string (GetId ()) + ".txt";
    myfile.open (fileName, fstream::app);
    myfile << 
        last_in_range_time << " " << 
        last_out_range_time << " " << 
        current_transmitted_count << " " <<
        current_range_transmitted_data << " " << 
        GetDataCurrentSize () << std::endl;
    myfile.close();

    //
    TxLog* log = new TxLog;
    log->interval = last_out_range_time - last_in_range_time;
    log->tx_count = current_transmitted_count;
    if (current_range_transmitted_data == -1) {
        log->tx_data = 0;
    }
    else {
        log->tx_data = current_range_transmitted_data;
    }
    log->left_data = GetDataCurrentSize ();

    txLogList.push_back (log);
};