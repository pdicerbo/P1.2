#!/bin/bash

#SBATCH --partition=mb
#SBATCH --job-name=session2-es0
#SBATCH --output=es0_%j.out
#SBATCH --error=es0_%j.err
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --time=00:10:00
#SBATCH --gres=gpu

LOW_FREQ=800000
HIGH_FREQ=1600000

srun sleep 10
export OMP_NUM_THREADS=1
srun --cpu-freq=$LOW_FREQ $HOME/session-3/es1.x
sleep 8
srun --cpu-freq=$HIGH_FREQ $HOME/session-3/es1.x
sleep 8
export OMP_NUM_THREADS=2
srun --cpu-freq=$LOW_FREQ $HOME/session-3/es1.x
sleep 8
srun --cpu-freq=$HIGH_FREQ $HOME/session-3/es1.x
echo "START RUNNING ON GPU"
sleep 8
srun --cpu-freq=$LOW_FREQ $HOME/session-3/es2
sleep 8
srun --cpu-freq=$HIGH_FREQ $HOME/session-3/es2
sleep 10
