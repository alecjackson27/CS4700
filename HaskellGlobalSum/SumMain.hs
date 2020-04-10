module Main where

import Data.Time.Clock (diffUTCTime, getCurrentTime)
import System.Environment (getArgs)
import System.Random (StdGen, getStdGen, randoms)
import Control.Parallel (par, pseq)
import GlobalSum


force :: [a] -> ()
force xs = go xs `pseq` ()
    where go (_:xs) = go xs
          go [] = 1

genRands :: Int -> StdGen -> [Int]
genRands n generator = force result `seq` result
                where
                    result =take n (randoms generator)

main = do
    args <- getArgs
    let count = read (head args)
    lst <- genRands count `fmap` getStdGen
    putStrLn $ "Computing global sum of " ++ show count ++ " numbers..."
    start <- getCurrentTime
    let summation = parGlobalSum lst
    putStrLn $ "Sum is " ++ show (summation)
    end <- getCurrentTime
    putStrLn $ "Computation took " ++ show (end `diffUTCTime` start) ++ " seconds."
