// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <algorithm>
#include <cmath>

#include "tnn/device/cpu/acc/cpu_layer_acc.h"
#include "tnn/utils/bfp16.h"
#include "tnn/utils/bfp16_utils.h"
#include "tnn/utils/dims_vector_utils.h"

namespace TNN_NS {

DECLARE_CPU_ACC(Relu, LAYER_RELU);

Status CpuReluLayerAcc::Reshape(const std::vector<Blob *> &inputs, const std::vector<Blob *> &outputs) {
    return TNN_OK;
}

Status CpuReluLayerAcc::Forward(const std::vector<Blob *> &inputs, const std::vector<Blob *> &outputs) {
    Blob *input_blob  = inputs[0];
    Blob *output_blob = outputs[0];
    int count         = DimsVectorUtils::Count(output_blob->GetBlobDesc().dims);
    auto data_type    = output_blob->GetBlobDesc().data_type;
    if (data_type == DATA_TYPE_FLOAT) {
        float *input_data  = static_cast<float *>(input_blob->GetHandle().base);
        float *output_data = static_cast<float *>(output_blob->GetHandle().base);
        for (int index = 0; index < count; ++index) {
            output_data[index] = std::max(0.0f, input_data[index]);
        }
    } else if (data_type == DATA_TYPE_BFP16) {
        bfp16_t *input_data  = static_cast<bfp16_t *>(input_blob->GetHandle().base);
        bfp16_t *output_data = static_cast<bfp16_t *>(output_blob->GetHandle().base);
        for (int index = 0; index < count; ++index) {
            output_data[index] = std::max(0.0f, (float)input_data[index]);
        }
    } else if (data_type == DATA_TYPE_INT8) {
        int8_t *input_data  = static_cast<int8_t *>(input_blob->GetHandle().base);
        int8_t *output_data = static_cast<int8_t *>(output_blob->GetHandle().base);
        for (int index = 0; index < count; ++index) {
            output_data[index] = std::max((int8_t)0, input_data[index]);
        }
    }
    return TNN_OK;
}

REGISTER_CPU_ACC(Relu, LAYER_RELU);

}  // namespace TNN_NS
