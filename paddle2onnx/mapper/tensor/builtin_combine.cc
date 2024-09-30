// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "paddle2onnx/mapper/tensor/builtin_combine.h"
#include "paddle/common/enforce.h"
#include "paddle/pir/include/core/builtin_op.h"
#include "paddle/pir/include/core/operation.h"

namespace paddle2onnx {
REGISTER_PIR_MAPPER(builtin_combine, BuiltinCombineMapper)

int64_t BuiltinCombineMapper::GetInputNum() {
    auto& op = pir_parser_->global_blocks_ops[pir_op_idx_];
    PADDLE_ENFORCE_EQ(
          op->isa<pir::CombineOp>(),
          true,
          common::errors::InvalidArgument(
            "The operator type must be builtin.combine, but the actual operator type is %s.",
            op->name()));
    return op->dyn_cast<pir::CombineOp>().inputs().size();
}

void BuiltinCombineMapper::Opset7() {
    auto output_info = GetOutput(0);
    int64_t input_num = GetInputNum();
    PADDLE_ENFORCE_EQ(
        input_num == output_info.size(),
        true,
        common::errors::InvalidArgument(
            "The number of inputs and outputs must be the same, but the actual "
            "input number is %d and output number is %d.",
            input_num, output_info.size()));
    for(int64_t i = 0; i < input_num; ++i) {
        auto input_info = GetInput(i);
        helper_->MakeNode("Identity", {input_info[0].name}, {output_info[i].name});
    }
}

}  // namespace paddle2onnx
