#!/bin/bash

./74hc595_init.sh

./74hc595_shift_reg_clear.sh
./74hc595_storage_reg.sh

./74hc595_set_data.sh 1

for i in `seq 1 8`;
do
    ./74hc595_shift_reg.sh
    ./74hc595_storage_reg.sh
    sleep 0.5
done

./74hc595_shift_reg_clear.sh
./74hc595_storage_reg.sh
