#!/bin/bash

cd TESTS
grep -e Maximum */time_reading*
grep -e Maximum */time_sequential*
grep -e Maximum */threads/*/time*
