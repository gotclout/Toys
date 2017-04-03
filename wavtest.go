package main

import (
  "bufio"
  _ "bytes"
  "fmt"
  "github.com/cryptix/wav"
  "os"
  _ "path/filepath"
  _ "strconv"
)

func RetrieveROM(filename string) ([]byte, error) {
  file, err := os.Open(filename)

  if err != nil {
    return nil, err
  }
  defer file.Close()

  stats, statsErr := file.Stat()
  if statsErr != nil {
    return nil, statsErr
  }

  var size int64 = stats.Size()
  bytes := make([]byte, size)

  bufr := bufio.NewReader(file)
  _, err = bufr.Read(bytes)

  return bytes, err
}

func main() {
  const (
    bits = 16
    rate = 44100
    inc  = 3
  )

  increments := make([]string, inc)
  increments[0] = "C:\\Users\\rfoster\\Desktop\\testdata\\0009.wav"
  increments[1] = "C:\\Users\\rfoster\\Desktop\\testdata\\0918.wav"
  increments[2] = "C:\\Users\\rfoster\\Desktop\\testdata\\1827.wav"

  of, _ := os.Create("of.wav")
  var wf = wav.File{
    Channels:        1,
    SampleRate:      44100,
    SignificantBits: 16,
  }

  wf.NewWriter(of)
  for i := 0; i < inc; i++ {
    fmt.Println("File: " + increments[i])
    f, _ := os.Open(increments[i])
    stat, _ := f.Stat()
    wr, _ := wav.NewReader(f, stat.Size())
    samps := wr.GetSampleCount()
    fmt.Print("Samples: ")
    fmt.Sprint(samps)
    fmt.Println()
    var s uint32
    for s = 0; s < samps; s++ {
      cs, _ := wr.ReadSample()
    }
  }
}
